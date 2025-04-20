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
#include "ogxc_ttf/ogxc_ttf.h"

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

static const int SCREEN_WIDTH = 720;
static const int SCREEN_HEIGHT = 480;

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

// TEGAN this is fucking chaos, clean it up
int main(void)
{
    int done = 0;
    SDL_Window *window;
    SDL_Event event;
    SDL_Surface *screenSurface, *imageSurface;

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

    XVideoSetMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, REFRESH_DEFAULT);

    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    // Init SDL
    if (SDL_VideoInit(NULL) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL video.\n");
        printSDLErrorAndReboot();
    }

    // Create application window
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

    ogxc_ttf_init("D:\\vegur-regular.ttf", window, screenSurface);
    SDL_Color font_color = {0x7F, 0xFF, 0x7F, 0xFF};

    // Draw main background
    SDL_BlitSurface(imageSurface, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);

    // Move these to header / consts
    const int header_main_x = 60;
    const int header_main_y = 22;
    const int header_online_x = 380;
    const int header_online_y = 22;

    ogxc_ttf_write("OGX Cloud - v0.1", header_main_x, header_main_y, font_color, 18);
    ogxc_ttf_write("Connected! (FTP) : 192.168.0.218:21", header_online_x, header_online_y, font_color, 18);

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

        Sleep(1000);
    }

    SDL_VideoQuit();

    return 0;
}
