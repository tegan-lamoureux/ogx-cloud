    // // FTP Work, in progress, Tegan - abstract to header/api
    // //   nxNetInit(NULL);

    // //   debugPrint("\n");
    // //   debugPrint("IP address.. %s\n", ip4addr_ntoa(netif_ip4_addr(g_pnetif)));
    // //   debugPrint("Mask........ %s\n",
    // //   ip4addr_ntoa(netif_ip4_netmask(g_pnetif))); debugPrint("Gateway.....
    // //   %s\n", ip4addr_ntoa(netif_ip4_gw(g_pnetif))); debugPrint("\n");

    // //   while (1) {
    // //     switch (state) {
    // //       case OGXC_DISCONNECTED: {
    // //         state = OGXC_CHECKING_ONLINE;

    // //         struct ip_addr resolved;
    // //         switch (dns_gethostbyname("www.google.com", &resolved,
    // //         smtp_serverFound,
    // //                                   NULL)) {
    // //           case ERR_OK:
    // //             // numeric or cached, returned in resolved
    // //             break;
    // //           case ERR_INPROGRESS:
    // //             break;
    // //           default:
    // //             debugPrint("NO INTERNET CONNECTION\n");
    // //             state = OGXC_IDLE;
    // //             break;
    // //         }

    // //         break;
    // //       }
    // //       case OGXC_CHECKING_ONLINE: {
    // //         NtYieldExecution();
    // //         break;
    // //       }
    // //       case OGXC_ONLINE: {
    // //         state = OGXC_CHECKING_FTP;
    // //         ftp_test();
    // //         break;
    // //       }
    // //       case OGXC_CHECKING_FTP:
    // //       case OGXC_IDLE:
    // //       case OGXC_CONNECTED: {
    // //         NtYieldExecution();
    // //         break;
    // //       }
    // //     }
    // //   }


    // static void ftp_retr_callback(void *arg, int result)
    // {
    //     lwftp_session_t *s = (lwftp_session_t *)arg;
    
    //     if (result == LWFTP_RESULT_INPROGRESS)
    //         return;
    
    //     if (result != LWFTP_RESULT_OK)
    //     {
    //         debugPrint("retr failed (%d)\n", result);
    //         return lwftp_close(s);
    //     }
    //     // Test is done
    //     lwftp_close(s);
    // }
    
    // static unsigned int data_sink(void *arg, const char *ptr, unsigned int len)
    // {
    //     static char myconfig[256];
    //     static unsigned int offset = 0;
    
    //     if (ptr)
    //     {
    //         snprintf(myconfig, 32, "%s", ptr);
    //         // debugPrint("DB: %s", myconfig);
    //     }
    //     return len;
    // }
    
    // static void ftp_connect_callback(void *arg, int result)
    // {
    //     lwftp_session_t *s = (lwftp_session_t *)arg;
    //     err_t error;
    
    //     if (result == LWFTP_RESULT_INPROGRESS)
    //         return;
    
    //     if (result != LWFTP_RESULT_LOGGED)
    //     {
    //         debugPrint("login failed (%d)\n", result);
    //         return lwftp_close(s);
    //     }
    
    //     // Continue with LIST request
    //     s->data_sink = data_sink;
    //     s->done_fn = ftp_retr_callback;
    //     s->remote_path = "";
    //     error = lwftp_list(s);
    //     if (error != LWFTP_RESULT_INPROGRESS)
    //     {
    //         debugPrint("lwftp_list failed (%d)\n", error);
    //     }
    // }
    
    // static void ftp_test(void)
    // {
    //     static lwftp_session_t s; // static content for the whole FTP session
    //     err_t error;
    
    //     // Initialize session data
    //     memset(&s, 0, sizeof(s));
    //     IP_ADDR4(&s.server_ip, 192, 168, 0, 204);
    //     s.server_port = 21;
    //     s.done_fn = ftp_connect_callback;
    //     s.user = "xbox";
    //     s.pass = "xbox";
    //     // We have no extra user data, simply use the session structure
    //     s.handle = &s;
    
    //     // Start the connection state machine
    //     error = lwftp_connect(&s);
    //     if (error != LWFTP_RESULT_INPROGRESS)
    //     {
    //         debugPrint("lwftp_connect failed (%d)\n", error);
    //     }
    //     // FTP session will continue with the connection callback
    // }
    