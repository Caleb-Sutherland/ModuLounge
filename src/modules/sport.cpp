#include <gtkmm.h>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>
#include "sport.h"
#include <math.h>

/// Destructor definition
Sport::~Sport(){};

/// function declarations - will have proper comments on their definitions farther down
static std::size_t writeFunction(char *ptr, std::size_t size, std::size_t num, char* out);
void formatDate(std::string *date);

void Sport::initializeSports(int refresh){
    /// retreive the sports news as a JSON full of articles
    this->articles = Sport::getSportsNews();  

    /// if the call isn't a refresh call, we must initialize all the labels that will eventually get filled with articles and displayed
    if(refresh == 0){
        this->art1.title = Gtk::Label();
        this->art2.title = Gtk::Label();
        //this->art3.title = Gtk::Label();

        this->art1.author = Gtk::Label();
        this->art2.author = Gtk::Label();
        //this->art3.author = Gtk::Label();

        this->art1.source = Gtk::Label();
        this->art2.source = Gtk::Label();
        //this->art3.source = Gtk::Label();

        this->art1.published = Gtk::Label();
        this->art2.published = Gtk::Label();
        //this->art3.published = Gtk::Label();

        this->art1.description = Gtk::Label();
        this->art2.description = Gtk::Label();
        //this->art3.description = Gtk::Label();  
    }

    /// if no articles were found, only set one label to display that information
    if(this->articles["articles"].size() == 0){
        this->art1.title.set_markup("<span size='large'><b>No Articles Could Be Found for this keyword!</b></span> \n");  
    }
    else{
        int index1, index2, index3;
        
        /// pick three articles from the retreived list at random (select their indexes in the JSON)
        if(this->articles["articles"].size() >= 1){
            /// set index one and two
            index1 = rand() % this->articles["articles"].size();
            index2 = rand() % this->articles["articles"].size();
            while(index2 == index1 && this->articles["articles"].size() >=2){
                index2 = rand() % this->articles["articles"].size();
            }
            /// set index 3 (if its in use)
            // index3 = rand() % this->articles["articles"].size();
            // while(index3 == index2 || index3 == index1 && this->articles["articles"].size() >=3){
            //     index3 = rand() % this->articles["articles"].size();
            // }
            /// if less than or equal to 3 articles are found in the whole JSON, identical articles will just be displayed
        }

        /// convert the json data into strings to be used later
        std::string des1 = this->articles["articles"][index1]["description"].asString();
        std::string des2 = this->articles["articles"][index2]["description"].asString();
        //std::string des3 = this->articles["articles"][index3]["description"].asString();

        std::string title1 = this->articles["articles"][index1]["title"].asString();
        std::string title2 = this->articles["articles"][index2]["title"].asString();
        //std::string title3 = this->articles["articles"][index3]["title"].asString();

        std::string author1 = this->articles["articles"][index1]["author"].asString();
        std::string author2 = this->articles["articles"][index2]["author"].asString();
        //std::string author3 = this->articles["articles"][index3]["author"].asString();

        std::string source1 = this->articles["articles"][index1]["source"]["name"].asString();
        std::string source2 = this->articles["articles"][index2]["source"]["name"].asString();
        //std::string source3 = this->articles["articles"][index3]["source"]["name"].asString();

        std::string published1 = this->articles["articles"][index1]["publishedAt"].asString();
        std::string published2 = this->articles["articles"][index2]["publishedAt"].asString();
        //std::string published3 = this->articles["articles"][index3]["publishedAt"].asString();

        /// call function to trim the end off of the date indicating the minute that it was published (looks nicer)
        formatDate(&published1);
        formatDate(&published2);
        //formatDate(&published3);

        /// some articles don't always have authors, so if they don't set a filler
        if(author1.length() == 0){
            author1 = "Author N/A";
        }
        if(author2.length() == 0){
            author2 = "Author N/A";
        }
        // if(author3.length() == 0){
        //     author3 = "Author N/A";
        // }
        
        int max_width = 70;     ///width of the articles
        /// set up all three articles to be displayed (this is one) all three have the same format, some 'Pango' is used to style them with markup a bit better
        this->art1.title.set_markup("<span size=\"large\"><b>\n" + title1 + "</b></span>");
        this->art1.title.set_max_width_chars(max_width);
        this->art1.title.set_line_wrap(true);

        this->art1.author.set_markup("<i>\n" + author1 + "</i>");
        this->art1.author.set_max_width_chars(max_width);
        this->art1.author.set_line_wrap(true);

        this->art1.source.set_label("----------------------\n" + source1);
        this->art1.source.set_max_width_chars(max_width);
        this->art1.source.set_line_wrap(true);

        this->art1.published.set_label("----------------------\n"+ published1);
        this->art1.published.set_max_width_chars(max_width);
        this->art1.published.set_line_wrap(true);

        this->art1.description.set_label("\n"+ des1);
        this->art1.description.set_max_width_chars(max_width);
        this->art1.description.set_line_wrap(true);

        /// second article
        this->art2.title.set_markup("<span size=\"large\"><b>\n" + title2 + "</b></span>");
        this->art2.title.set_max_width_chars(max_width);
        this->art2.title.set_line_wrap(true);

        this->art2.author.set_markup("<i>\n" + author2 + "</i>");
        this->art2.author.set_max_width_chars(max_width);
        this->art2.author.set_line_wrap(true);

        this->art2.source.set_label("----------------------\n" + source2);
        this->art2.source.set_max_width_chars(max_width);
        this->art2.source.set_line_wrap(true);

        this->art2.published.set_label("----------------------\n"+ published2);
        this->art2.published.set_max_width_chars(max_width);
        this->art2.published.set_line_wrap(true);

        this->art2.description.set_label("\n"+ des2);
        this->art2.description.set_max_width_chars(max_width);
        this->art2.description.set_line_wrap(true);

        /// third article (commented out as it made the box to big --kept here just in case)
        // this->art3.title.set_markup("<span size=\"large\"><b>\n" + title3 + "</b></span>");
        // this->art3.title.set_max_width_chars(max_width);
        // this->art3.title.set_line_wrap(true);

        // this->art3.author.set_markup("<i>\n" + author3 + "</i>");
        // this->art3.author.set_max_width_chars(max_width);
        // this->art3.author.set_line_wrap(true);

        // this->art3.source.set_label("----------------------\n" + source3);
        // this->art3.source.set_max_width_chars(max_width); 
        // this->art3.source.set_line_wrap(true);

        // this->art3.published.set_label("----------------------\n"+ published3);
        // this->art3.published.set_max_width_chars(max_width);
        // this->art3.published.set_line_wrap(true);

        // this->art3.description.set_label("\n"+ des3);
        // this->art3.description.set_max_width_chars(max_width);
        // this->art3.description.set_line_wrap(true);
    }
}


void Sport::populateModule(){
    std::cout << "Start populating sport Module" << std::endl;

    /// set up a vertical box
    this->vboxMain = Gtk::VBox();    
    /// remove whats originally in the main frame
    this->frame.remove();
    /// add the vertical box we will be using
    this->frame.add(this->vboxMain);

    /// align all the articles to the left side of the page
    this->art1.title.set_halign(Gtk::ALIGN_START);
    this->art1.author.set_halign(Gtk::ALIGN_START);
    this->art1.published.set_halign(Gtk::ALIGN_START);
    this->art1.description.set_halign(Gtk::ALIGN_START);
    this->art1.source.set_halign(Gtk::ALIGN_START);

    this->art2.title.set_halign(Gtk::ALIGN_START);
    this->art2.author.set_halign(Gtk::ALIGN_START);
    this->art2.published.set_halign(Gtk::ALIGN_START);
    this->art2.description.set_halign(Gtk::ALIGN_START);
    this->art2.source.set_halign(Gtk::ALIGN_START);

    //commented out --kept in case of future use
    // this->art3.title.set_halign(Gtk::ALIGN_START);
    // this->art3.author.set_halign(Gtk::ALIGN_START);
    // this->art3.published.set_halign(Gtk::ALIGN_START);
    // this->art3.description.set_halign(Gtk::ALIGN_START);
    // this->art3.source.set_halign(Gtk::ALIGN_START);

    /// add all the articles info to the box so that it is displayed
    this->vboxMain.pack_start(this->art1.title, Gtk::PACK_SHRINK,0);
    this->vboxMain.pack_start(this->art1.author, Gtk::PACK_SHRINK,0);
    this->vboxMain.pack_start(this->art1.source, Gtk::PACK_SHRINK,0);
    this->vboxMain.pack_start(this->art1.published, Gtk::PACK_SHRINK,0);
    this->vboxMain.pack_start(this->art1.description, Gtk::PACK_SHRINK,0);

    this->vboxMain.pack_start(this->art2.title, Gtk::PACK_SHRINK,0);
    this->vboxMain.pack_start(this->art2.author, Gtk::PACK_SHRINK,0);
    this->vboxMain.pack_start(this->art2.source, Gtk::PACK_SHRINK,0);
    this->vboxMain.pack_start(this->art2.published, Gtk::PACK_SHRINK,0);
    this->vboxMain.pack_start(this->art2.description, Gtk::PACK_SHRINK,0);

    //kept in case of future use
    // this->vboxMain.pack_start(this->art3.title, Gtk::PACK_SHRINK,0);
    // this->vboxMain.pack_start(this->art3.author, Gtk::PACK_SHRINK,0);
    // this->vboxMain.pack_start(this->art3.source, Gtk::PACK_SHRINK,0);
    // this->vboxMain.pack_start(this->art3.published, Gtk::PACK_SHRINK,0);
    // this->vboxMain.pack_start(this->art3.description, Gtk::PACK_SHRINK,0);

    /// create a thread that runs a timer in the background that refreshes the page when necessary
    std::thread sportThread(&Sport::refresher, this);
    sportThread.detach();

    std::cout << "Finished populating custom Module" << std::endl;
    
}


Json::Value Sport::getSportsNews(){
    /// replace spaces in the keyword to + signs so the url works
    for(int x = 0; this->sport[x]; x++){
        if(this->sport[x] == ' '){
            this->sport[x] = '+';
        }
    }

    ///Two api keys in case one gets over used and stops working
    ///78572ce08bad450c9a68185c8b7fc9c5
    ///573cff74201a406692be1fd831a14588

    /// url to retrieve information from using libcurl
    const std::string APIKEY = "573cff74201a406692be1fd831a14588";
    const std::string URL = "http://newsapi.org/v2/everything?q=" + this->sport + "&language=en&sortBy=publishedAt&apiKey=" + APIKEY;

    /// initialize curl 
    CURL* curl = curl_easy_init();
    if (curl) {
        /// set up the curl url and other parameters
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
           
        /// set up a function to write the response to and a stringstream to write the response to
        std::stringstream response_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        /// perform the curl
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
       
        Json::Value data;
        Json::CharReaderBuilder reader;
        std::string errs;
        
        /// convert the data to a JSON so it can be easily parsed and manipulated
        if(Json::parseFromStream(reader, response_string, &data, &errs)){
            return data;
        }
        throw std::string("No articles could be found for your input. Please enter a more recognizable team or sport.");
    } 
}

/**
 * @brief WriteFunction simply handles the return of the libcurl, it is necessary for the libcurl to work, but we don't use it for anything else
 * Detailed Description: It is necessary for the libcurl to work, it takes in the response_string and turns it into a stringstream output. 
 * @param ptr: the response stream
 * @param size: the size of the string stream
 * @param num: number of streams
 * @param out: the output stream
 * @return size of the output stream
 */
static std::size_t writeFunction(char *ptr, std::size_t size, std::size_t num, char* out) {
    std::string data (ptr, (std::size_t) size * num);
    *((std::stringstream*) out) << data;
    return size * num;
}

void Sport::refresher(){
    while(true){
        std::this_thread::sleep_for (std::chrono::seconds(20));
        Sport::initializeSports(1);
    }
}

/**
 * @brief formatDate takes in a date and substrings it so that only the day month year are in the text
 * Detailed Description: loops through a string and finds the first letters, takes everything up to that letter so that the date only contains what
 * we need and not extra data (looks cleaner). Uses a pointer so that it doesn't need to return anything (pass by reference)
 * @param date: date to be formatted, it is a pointer so it is passed by reference and the function doesnt need to return anything.
 */
void formatDate(std::string *date){
    if(date->length() != 0){
        std::string temp = *date;
        int i = 0;
        while((int) temp[i] < 65){
            i++;
        }
        *date = temp.substr(0, i);
    }
}

