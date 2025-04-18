#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <lwip/debug.h>
#include <lwip/dns.h>
#include <lwip/tcpip.h>
#include <nxdk/net.h>
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#include "lwftp/lwftp.h"

typedef enum
{
    OGXC_DISCONNECTED = 0,
    OGXC_CHECKING_ONLINE,
    OGXC_ONLINE,
    OGXC_CHECKING_FTP,
    OGXC_CONNECTED,
    OGXC_IDLE,
} ogxc_state;

ogxc_state state = OGXC_DISCONNECTED;
extern struct netif *g_pnetif;

static void printSDLErrorAndReboot(void)
{
    debugPrint("SDL_Error: %s\n", SDL_GetError());
    debugPrint("Rebooting in 5 seconds.\n");
    Sleep(5000);
    XReboot();
}

static void printIMGErrorAndReboot(void)
{
    debugPrint("SDL_Image Error: %s\n", IMG_GetError());
    debugPrint("Rebooting in 5 seconds.\n");
    Sleep(5000);
    XReboot();
}

void smtp_serverFound(const char *name, const struct ip_addr *ipaddr, void *arg)
{
    if (ipaddr)
    {
        debugPrint("Online! (Google DNS Resolved: %s)\n\n", ipaddr_ntoa(ipaddr));
        state = OGXC_ONLINE;
    }
}

static void ftp_retr_callback(void *arg, int result)
{
    lwftp_session_t *s = (lwftp_session_t *)arg;

    if (result == LWFTP_RESULT_INPROGRESS)
        return;

    if (result != LWFTP_RESULT_OK)
    {
        debugPrint("retr failed (%d)\n", result);
        return lwftp_close(s);
    }
    // Test is done
    lwftp_close(s);
}

static unsigned int data_sink(void *arg, const char *ptr, unsigned int len)
{
    static char myconfig[256];
    static unsigned int offset = 0;

    if (ptr)
    {
        snprintf(myconfig, 32, "%s", ptr);
        // debugPrint("DB: %s", myconfig);
    }
    return len;
}

static void ftp_connect_callback(void *arg, int result)
{
    lwftp_session_t *s = (lwftp_session_t *)arg;
    err_t error;

    if (result == LWFTP_RESULT_INPROGRESS)
        return;

    if (result != LWFTP_RESULT_LOGGED)
    {
        debugPrint("login failed (%d)\n", result);
        return lwftp_close(s);
    }

    // Continue with LIST request
    s->data_sink = data_sink;
    s->done_fn = ftp_retr_callback;
    s->remote_path = "";
    error = lwftp_list(s);
    if (error != LWFTP_RESULT_INPROGRESS)
    {
        debugPrint("lwftp_list failed (%d)\n", error);
    }
}

static void ftp_test(void)
{
    static lwftp_session_t s; // static content for the whole FTP session
    err_t error;

    // Initialize session data
    memset(&s, 0, sizeof(s));
    IP_ADDR4(&s.server_ip, 192, 168, 0, 204);
    s.server_port = 21;
    s.done_fn = ftp_connect_callback;
    s.user = "xbox";
    s.pass = "xbox";
    // We have no extra user data, simply use the session structure
    s.handle = &s;

    // Start the connection state machine
    error = lwftp_connect(&s);
    if (error != LWFTP_RESULT_INPROGRESS)
    {
        debugPrint("lwftp_connect failed (%d)\n", error);
    }
    // FTP session will continue with the connection callback
}

// Screen dimension constants
static const int SCREEN_WIDTH = 720;
static const int SCREEN_HEIGHT = 480;

// TEGAN this is fucking chaos, clean it up
int main(void)
{
    int done = 0;
    SDL_Window *window;
    SDL_Event event;
    SDL_Surface *screenSurface, *imageSurface;
    int initialized_TTF = -1;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    void *vm_p = 0;
    VIDEO_MODE vm;
    int vm_timeout = 10;
    while (XVideoListModes(&vm, 0, 0, &vm_p) && vm_timeout-- > 0)
    {
        // debugPrint("Video mode %d x %d, %dbpp, %dhz\n", vm.width, vm.height,
        // vm.bpp, vm.refresh);
        // Tegan - handle modes here, pick highest rez (Or just force and only
        // support 720p? it's 2024, I think all TVs support this)
    }

    XVideoSetMode(720, 480, 32, REFRESH_DEFAULT);

    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    if (SDL_VideoInit(NULL) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL video.\n");
        printSDLErrorAndReboot();
    }

    initialized_TTF = TTF_Init();
    if (initialized_TTF != 0)
    {
        debugPrint("TTF_Init failed: %s", TTF_GetError());
        Sleep(2000);
    }

    TTF_Font *font;
    const char *font_path = "D:\\vegur-regular.ttf";
    const int font_size = 18;
    font = TTF_OpenFont(font_path, font_size);
    if (font == NULL)
    {
        debugPrint("Couldn't load font: %s", TTF_GetError());
        Sleep(2000);
    }

    window = SDL_CreateWindow("Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        debugPrint("Window could not be created!\n");
        SDL_VideoQuit();
        printSDLErrorAndReboot();
    }

    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't intialize SDL_image.\n");
        SDL_VideoQuit();
        printIMGErrorAndReboot();
    }

    screenSurface = SDL_GetWindowSurface(window);
    if (!screenSurface)
    {
        SDL_VideoQuit();
        printSDLErrorAndReboot();
    }

    imageSurface = IMG_Load("D:\\bg_720_480.jpg");
    if (!imageSurface)
    {
        SDL_VideoQuit();
        printIMGErrorAndReboot();
    }

    int drawOnce = 0;
    while (!done)
    {
        /* Check for events */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = 1;
                break;
            default:
                break;
            }
        }

        SDL_BlitSurface(imageSurface, NULL, screenSurface, NULL);
        SDL_UpdateWindowSurface(window);

        // Test render text, Tegan - move this to a function
        if (drawOnce)
        {
            drawOnce = 0;

            SDL_Color font_color = {0x7F, 0xFF, 0x7F, 0xFF};
            screenSurface = TTF_RenderText_Blended(font, "hello world :3", font_color);
            TTF_CloseFont(font);
            if (screenSurface == NULL)
            {
                debugPrint("TTF_RenderText failed: %s", TTF_GetError());
                Sleep(2000);
            }

            texture = SDL_CreateTextureFromSurface(renderer, screenSurface);
            SDL_FreeSurface(screenSurface);
            if (texture == NULL)
            {
                debugPrint("Couldn't create texture: %s\n", SDL_GetError());
                Sleep(2000);
            }

            SDL_Rect outputPosition;
            int window_height, window_width;
            SDL_GetWindowSize(window, &window_width, &window_height);
            SDL_QueryTexture(texture, NULL, NULL, &outputPosition.w, &outputPosition.h);
            outputPosition.x = window_width / 2 - outputPosition.w / 2;
            outputPosition.y = window_height / 2 - outputPosition.h / 2;

            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, &outputPosition);
            SDL_RenderPresent(renderer);
        }

        Sleep(1000);
    }

    SDL_VideoQuit();

    // FTP Work, in progress, Tegan - abstract to header/api
    //   nxNetInit(NULL);

    //   debugPrint("\n");
    //   debugPrint("IP address.. %s\n", ip4addr_ntoa(netif_ip4_addr(g_pnetif)));
    //   debugPrint("Mask........ %s\n",
    //   ip4addr_ntoa(netif_ip4_netmask(g_pnetif))); debugPrint("Gateway.....
    //   %s\n", ip4addr_ntoa(netif_ip4_gw(g_pnetif))); debugPrint("\n");

    //   while (1) {
    //     switch (state) {
    //       case OGXC_DISCONNECTED: {
    //         state = OGXC_CHECKING_ONLINE;

    //         struct ip_addr resolved;
    //         switch (dns_gethostbyname("www.google.com", &resolved,
    //         smtp_serverFound,
    //                                   NULL)) {
    //           case ERR_OK:
    //             // numeric or cached, returned in resolved
    //             break;
    //           case ERR_INPROGRESS:
    //             break;
    //           default:
    //             debugPrint("NO INTERNET CONNECTION\n");
    //             state = OGXC_IDLE;
    //             break;
    //         }

    //         break;
    //       }
    //       case OGXC_CHECKING_ONLINE: {
    //         NtYieldExecution();
    //         break;
    //       }
    //       case OGXC_ONLINE: {
    //         state = OGXC_CHECKING_FTP;
    //         ftp_test();
    //         break;
    //       }
    //       case OGXC_CHECKING_FTP:
    //       case OGXC_IDLE:
    //       case OGXC_CONNECTED: {
    //         NtYieldExecution();
    //         break;
    //       }
    //     }
    //   }

    return 0;
}
