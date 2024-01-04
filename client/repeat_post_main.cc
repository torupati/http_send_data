#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
            fprintf(stderr, "curl_easy_setopt(CURLOPT_URL) failed: %s\n", curl_easy_strerror(res));
        }
        for(int i=0; i< 3; i++){
            char msgbuf[1024];
            sprintf(msgbuf, "foo=%d&hoge=aaa", 1000*i);
            res = curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, msgbuf);
            if(res != CURLE_OK){
                fprintf(stderr, "curl_easy_setopt(CURLOPT_POSTFIELDS) failed: %s\n", curl_easy_strerror(res));
            }
            res = curl_easy_perform(easy_handle);
            if(res != CURLE_OK){
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            struct timespec req = {.tv_sec=1, .tv_nsec=0};
            nanosleep(&req, NULL);
        }
        // send binary data
        res = curl_easy_setopt(easy_handle, CURLOPT_URL, "http://127.0.0.1:8080/bindata");
        if(res == CURLE_OK){
            printf("OK set url\n");
        }else{
            fprintf(stderr, "curl_easy_setopt(CURLOPT_URL) failed: %s\n", curl_easy_strerror(res));
        }
        uint32_t length_of_data = 1024*1024*3;
        unsigned char* data = (unsigned char*)malloc(length_of_data);
        //for(uint8_t j=0; j<256; j++) data[j] = j;
        //res = curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, length_of_data);
        res = curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE_LARGE, length_of_data);
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_setopt(CURLOPT_POSTFIELDSIZE) failed: %s\n", curl_easy_strerror(res));
        }
        printf("a2\n");
        res = curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, data);
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_setopt(CURLOPT_POSTFIELDS) failed: %s\n", curl_easy_strerror(res));
        }
        res = curl_easy_perform(easy_handle);
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(easy_handle);
    }
    curl_global_cleanup();
    return 0;
}
