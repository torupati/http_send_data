#include <time.h>
#include <curl/curl.h>

int main(void)
{
    curl_global_init(CURL_GLOBAL_ALL);
    // https://curl.se/libcurl/c/curl_global_init.html
    printf("libcurl version %s\n", curl_version());

    CURL *easy_handle = curl_easy_init();
    /* set URL to operate on */
    if(easy_handle){
        CURLcode res = curl_easy_setopt(easy_handle, CURLOPT_URL, "http://127.0.0.1:8080/sample1");
        if(res == CURLE_OK){
            printf("OK set url\n");
        }else{
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        for(int i=0; i< 10; i++){
            char msgbuf[1024];
            sprintf(msgbuf, "foo=%d&hoge=aaa", 1000*i);
            res = curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, msgbuf);
            if(res != CURLE_OK){
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            res = curl_easy_perform(easy_handle);
            if(res != CURLE_OK){
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            struct timespec req = {.tv_sec=1, .tv_nsec=0};
            nanosleep(&req, NULL);
        }
        curl_easy_cleanup(easy_handle);
    }
    curl_global_cleanup();
    return 0;
}
