#include <curl/curl.h>
#include <string>

size_t WriteCallback(char *contents, size_t size, size_t nmemb, std::string *out)
{
    out->append(contents);
    return size * nmemb;
}

std::string Get(const std::string &url)
{
    CURL *curl = curl_easy_init();
    std::string response;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}
