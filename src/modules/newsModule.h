#ifndef NEWSMODULE_H
#define NEWSMODULE_H
#include <stdio.h>
#include<iostream>
#include <curl/curl.h>
#include <string>
#include <cstring>
#include<jsoncpp/json/json.h>
#include<exception>
#include <gtkmm.h>
#include "../module.h"
/**
* @class NewsModule
* @author Ryan Harlen
* @brief A subclass of the Module class.
* This class is used to create a module that will disaplay relevant news information.
* News can be searched for by category, source, or topic.
* If found, source, title, date, description and content of the news found will be displayed to the screen.
*
*/
class NewsModule : public Module
{
private:
    struct jsonNews
    {
        public:
        std::string description;
        std::string date;
        std::string source;
        std::string title;
        std::string content;
    };
    struct newsData
    {
        public:
        char *data;
        size_t size;
    };
    /**
    * @param passed in through the use of curl_easy_setopt
    * @return the size of the data being saved
    * @brief required when using curl to save the data retrived so that it can be used later
    */
    static size_t saveData(void *contents, size_t size, size_t nmemb, void *userp);
    /**
    * @param a struct to store all important news data, and the associated data that will be save in the struct
    * @return none
    * @brief store the data in the object's jsonNews struct.
    * This allows for easy access to the data whenever required
    * Default values are provided if some values are missing
    */
    void saveJson(jsonNews *jNews, newsData news);
    std::string searchString;
    jsonNews newsToDisplay;


public:
    /**
    * @param the string used to search the news API, name of the module, width and height of the module
    * @return none
    * @brief calls the default module constructor and sets the value of searchString, so that the news API can be searched
    */
    NewsModule(std::string toSearchString, std::string name, int width, int height) : Module(name, width, height){this->searchString = toSearchString;};

    /**
    * @param none
    * @return none
    * @brief Retrieves news based off of user specifications chosen prior.
    * Adds visual elements to the module that will be displayed on the screen
    */
    void populateModule();

    /// Member widgets:
    Gtk::Label sourceLabel, titleLabel, dateLabel, descLabel, contentLabel;
    Gtk::VBox box;
};

#endif // NEWSMODULE_H
