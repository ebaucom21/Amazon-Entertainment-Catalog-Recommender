/*
-------------------------------
Main file to run entire project
-------------------------------
Includes all UX related tasks and integrates both data structures (KD-tree and Red-Black tree)
to create fully complete project :)
*/

//include files
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include "Balanced-KD.cpp"
#include "Balanced-RB.cpp"
#include "CSV-Extractor.cpp"
using namespace std;

//functions for each individual window
int welcome_window();
int question_window1();
int movie_window(int num_recs,string genre);
int show_window(int num_recs,string genre);
int Results_window(int num_recs, vector<string> info);

//global variables to be used in tree creations
vector<vector<vector<string>>> global_data;
balancedKD* global_kdTree = nullptr;
RedBlackTree* global_redblackTree = nullptr;

//function for setting text in center of screen
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

/*--------------------------------
 * EVERYTHING FOR WELCOME WINDOW
 * -------------------------------
*/
int welcome_window() {
    //dimensions
    int width = 1600;
    int height = 900;
    //rendering window
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);

    //title text
    sf::Font textFont;
    textFont.loadFromFile("../files/AmazonEmber_Rg.ttf");
    sf::Text title;
    title.setFont(textFont);
    title.setString("Amazon Prime Entertainment Catalog Recommender!");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    int titlex = width/2;
    int titley = (height/2) - 60;
    setText(title, titlex, titley);

    //top image
    sf::Texture main_image;
    main_image.loadFromFile("../files/images/AMAZON LOGO.jpg");
    sf::Sprite main_imageLogo(main_image);
    main_imageLogo.setTexture(main_image);
    main_imageLogo.setScale(sf::Vector2f(2, 2));
    main_imageLogo.setPosition(sf::Vector2f(297, -190));

    //start button
    sf::CircleShape start_button(60);
    start_button.setPosition(sf::Vector2f(550,680));
    start_button.setScale(sf::Vector2f(4, 1));
    start_button.setFillColor(sf::Color(255,153,0));
    sf::Text start_button_text("Start Now", textFont);
    start_button_text.setCharacterSize(60);
    start_button_text.setFillColor(sf::Color::White);
    start_button_text.setPosition(sf::Vector2f(650,700));

//screen usage
    while (window.isOpen()) {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Request for closing the window
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            // handles events for things clicked with mouse
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);
                    if (start_button.getGlobalBounds().contains(mouse.x,mouse.y)) {
                        window.close();
                        question_window1();
                    }
                }
            }
        }
        //render everything in this window
        window.clear(sf::Color(35,47,65)); //background color
        window.draw(main_imageLogo);
        window.draw(start_button);
        window.draw(start_button_text);
        window.draw(title);
        window.display();
    }
}


//function for highlight number of reccomendations choice
void highlightBox(sf::RectangleShape& box) {
    box.setFillColor(sf::Color(255, 153, 0)); // Amazon orange
}


/*--------------------------------
 * EVERYTHING FOR PRIMARY QUESTION WINDOW
 * -------------------------------
*/
int question_window1() {
    //dimensions
    int width = 1600;
    int height = 900;
    //rendering window
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);

    //array of answer choices for genres
    string genre_array[12] = {"Kids", "Action and adventure", "Comedy", "Documentary", "Horror", "Drama",
                        "Mystery and thrillers", "Romance", "Fantasy", "Science fiction", "Historical", "Anime"};
    // Load font
    sf::Font textFont;
    textFont.loadFromFile("../files/AmazonEmber_Rg.ttf");

    // Question 1 text (how many recommendations)
    sf::Text Recomend_amount_question;
    Recomend_amount_question.setFont(textFont);
    Recomend_amount_question.setString("How many recommendations would you like?");
    Recomend_amount_question.setCharacterSize(52);
    Recomend_amount_question.setFillColor(sf::Color::White);
    Recomend_amount_question.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect Recomend_amount_Bounds = Recomend_amount_question.getLocalBounds();
    Recomend_amount_question.setOrigin(Recomend_amount_Bounds.width / 2, Recomend_amount_Bounds.height / 2);
    Recomend_amount_question.setPosition(width / 2, 100);

    // Choice boxes (1–10)
    const int boxCount = 10;
    const float boxWidth = 100;
    const float boxHeight = 120;
    const float spacing = 30;
    const float startX = (width - (boxCount * boxWidth + (boxCount - 1) * spacing)) / 2;
    const float startY = 160;

    //create box graphics
    sf::RectangleShape boxes[boxCount];
    sf::Text numbers[boxCount];
    int selectedIndex = 0;  // Nothing selected at start
    //create each individual box on the screen with their numbers
    for (int i = 0; i < boxCount; ++i) {
        boxes[i].setSize(sf::Vector2f(boxWidth, boxHeight));
        boxes[i].setFillColor(sf::Color::White);
        boxes[i].setPosition(startX + i * (boxWidth + spacing), startY);

        numbers[i].setFont(textFont);
        numbers[i].setString(to_string(i + 1));
        numbers[i].setCharacterSize(48);
        numbers[i].setFillColor(sf::Color::Black);

        sf::FloatRect textRect = numbers[i].getLocalBounds();
        numbers[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        numbers[i].setPosition(boxes[i].getPosition().x + boxWidth / 2.0f, boxes[i].getPosition().y + boxHeight / 2.0f + 5);
    }

    // Question 2 (Genre question)
    sf::Text genreQuestion;
    genreQuestion.setFont(textFont);
    genreQuestion.setString("What is your favorite genre?");
    genreQuestion.setCharacterSize(52);
    genreQuestion.setFillColor(sf::Color::White);
    genreQuestion.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect genreBounds = genreQuestion.getLocalBounds();
    genreQuestion.setOrigin(genreBounds.width / 2, genreBounds.height / 2);
    genreQuestion.setPosition(width / 2, startY + boxHeight + 40);

    // Load 12 genre images
    const int imageCount = 12;
    const int imagesPerRow = 6;
    const float originalImageWidth = 400.0f;
    const float originalImageHeight = 240.0f;
    const float scaleFactor = 0.5f;
    const float scaledWidth = originalImageWidth * scaleFactor;
    const float scaledHeight = originalImageHeight * scaleFactor;
    const float imageSpacingX = (width - (imagesPerRow * scaledWidth)) / (imagesPerRow + 1);
    const float imageSpacingY = 30;
    const float imageStartY = genreQuestion.getPosition().y + 50;
    int selectedGenreIndex = 0;

    //vector with image names to be loaded in one by one
    vector<string> imageFiles = {
        "Cat_1.png", "Action_cat.png", "Comedy_cat.png", "Documentary_cat.png", "Horror_cat.png", "Drama_cat.png",
        "Mystery_cat.png", "Romance_cat.png", "Fantasy_cat.png", "SciFi_cat.png", "Historical_cat.png", "Anime_cat.png"
    };

    //vectors for handeling image rendering in window
    vector<sf::Texture> genre_image_textures(imageCount);
    vector<sf::Sprite> genre_sprites(imageCount);

    //load each image in from file path
    for (int i = 0; i < imageCount; ++i) {
        string path = "../files/images/" + imageFiles[i];
        if (!genre_image_textures[i].loadFromFile(path)) {
            cerr << "Failed to load image: " << path << endl;
            continue;
        }

        //create sprites for genre images
        genre_sprites[i].setTexture(genre_image_textures[i]);
        genre_sprites[i].setScale(0.5f, 0.5f); // scale to 50%
        //auto space images
        float x = imageSpacingX + (i % imagesPerRow) * (scaledWidth + imageSpacingX);
        float y = imageStartY + (i / imagesPerRow) * (scaledHeight + imageSpacingY);
        genre_sprites[i].setPosition(x, y);
        //save positions
        vector<sf::FloatRect> imageBounds(imageCount);
        imageBounds[i] = genre_sprites[i].getGlobalBounds();
    }

//3rd question (what type of entertainment)
    sf::Text typeQuestion;
    typeQuestion.setFont(textFont);
    typeQuestion.setString("Are you interested in finding movies or shows?");
    typeQuestion.setCharacterSize(48);
    typeQuestion.setFillColor(sf::Color::White);
    typeQuestion.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect typeBounds = typeQuestion.getLocalBounds();
    typeQuestion.setOrigin(typeBounds.width / 2, typeBounds.height / 2);
    typeQuestion.setPosition(width / 2, imageStartY + 2 * (scaledHeight + imageSpacingY) + 30);

    //creating bubble choices for selection (movie or show)
    //movie
    sf::Text movieOption, showOption;
    movieOption.setFont(textFont);
    movieOption.setString("Movies");
    movieOption.setCharacterSize(36);
    movieOption.setFillColor(sf::Color::White);
    movieOption.setPosition(width / 2 - 40, typeQuestion.getPosition().y + 50);
    //show
    showOption.setFont(textFont);
    showOption.setString("Shows");
    showOption.setCharacterSize(36);
    showOption.setFillColor(sf::Color::White);
    showOption.setPosition(width / 2 - 40, movieOption.getPosition().y + 50);
    //buttons
    //movie
    sf::CircleShape movieCircle(10);  // radius = 10
    movieCircle.setOutlineColor(sf::Color::White);
    movieCircle.setOutlineThickness(2);
    movieCircle.setFillColor(sf::Color::Transparent);
    movieCircle.setPosition(width / 2 - 80, movieOption.getPosition().y + 10);
    //show
    sf::CircleShape showCircle = movieCircle;
    showCircle.setPosition(width / 2 - 80, showOption.getPosition().y + 10);

    int selectedTypeIndex = 0;  // 0 = Movies, 1 = Shows

    // Next button
    sf::CircleShape next_button(30);
    next_button.setPosition(sf::Vector2f(1300,790));
    next_button.setScale(sf::Vector2f(4, 1));
    next_button.setFillColor(sf::Color(255,153,0));
    //next button text
    sf::Text next_button_text("Continue", textFont);
    next_button_text.setCharacterSize(36);
    next_button_text.setFillColor(sf::Color::White);
    sf::FloatRect sbBounds = next_button_text.getLocalBounds();
    next_button_text.setOrigin(sbBounds.width / 2, sbBounds.height / 2);
    next_button_text.setPosition(1420,810);

    // Start Over button
    sf::CircleShape start_over_button(30);
    start_over_button.setPosition(sf::Vector2f(70,790));
    start_over_button.setScale(sf::Vector2f(4, 1));
    start_over_button.setFillColor(sf::Color(255,153,0));
    //start over button text
    sf::Text start_over_text("Start Over", textFont);
    start_over_text.setCharacterSize(36);
    start_over_text.setFillColor(sf::Color::White);
    sf::FloatRect sobBounds = start_over_text.getLocalBounds();
    start_over_text.setOrigin(sobBounds.width / 2, sobBounds.height / 2);
    start_over_text.setPosition(190,810);

//HANDLING ALL EVENTS IN THIS WINDOW
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }

            // Detect mouse click on a box for number selection
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                //Switch index based on selection
                for (int i = 0; i < boxCount; ++i) {
                    if (boxes[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex = i;
                        break;
                    }
                }
            }
        }

        // Update box colors based on selected index
        for (int i = 0; i < boxCount; ++i) {
            if (i == selectedIndex)
                boxes[i].setFillColor(sf::Color(255, 153, 0));
            else
                boxes[i].setFillColor(sf::Color::White);
        }

        // Detect mouse click on selections in this window
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {

            //mouse position
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // Number box selection
            for (int i = 0; i < boxCount; ++i) {
                if (boxes[i].getGlobalBounds().contains(mousePos)) {
                    selectedIndex = i;
                }
            }

            // Genre image selection
            for (int i = 0; i < imageCount; ++i) {
                if (genre_sprites[i].getGlobalBounds().contains(mousePos)) {
                    selectedGenreIndex = i; // ONLY update on click
                }
            }

            //bubble choice selection
            if (movieCircle.getGlobalBounds().contains(mousePos)) {
                selectedTypeIndex = 0;
            } else if (showCircle.getGlobalBounds().contains(mousePos)) {
                selectedTypeIndex = 1;
            }

            //move on to next screen determined here by clicking next button
            if (next_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                //determines which screen to go to (to ask different type specific questions) based on type selection and passes in parameters
                if (selectedTypeIndex == 0) {
                    window.close();
                    movie_window((selectedIndex + 1), genre_array[selectedGenreIndex]);
                } else {
                    window.close();
                    show_window((selectedIndex + 1), genre_array[selectedGenreIndex]);
                }
                return 0;
            }
            //starts over back to welcome window
            if (start_over_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                window.close();
                welcome_window();
                return 0;
            }
        }

//RENDERING OBJECTS INTO WINDOW
        window.clear(sf::Color(35, 47, 65)); // Amazon gray
        window.draw(Recomend_amount_question);

        //each box for question 1
        for (int i = 0; i < boxCount; ++i) {
            window.draw(boxes[i]);
            window.draw(numbers[i]);
        }

        //Question 2 User selection updates
        window.draw(genreQuestion);

        for (int i = 0; i < imageCount; ++i) {
            window.draw(genre_sprites[i]);
        }
        // Draw selection outline if a genre is selected
        if (selectedGenreIndex != -1) {
            sf::FloatRect bounds = genre_sprites[selectedGenreIndex].getGlobalBounds();

            sf::RectangleShape genre_image_outline;
            genre_image_outline.setSize(sf::Vector2f(bounds.width, bounds.height));
            genre_image_outline.setPosition(bounds.left, bounds.top);
            genre_image_outline.setFillColor(sf::Color::Transparent);
            genre_image_outline.setOutlineThickness(4);
            genre_image_outline.setOutlineColor(sf::Color(255, 153, 0)); // Amazon orange

            window.draw(genre_image_outline);
        }

        // Apply orange fill for type selection if selected, transparent otherwise
        movieCircle.setFillColor(selectedTypeIndex == 0 ? sf::Color(255, 153, 0) : sf::Color::Transparent);
        showCircle.setFillColor(selectedTypeIndex == 1 ? sf::Color(255, 153, 0) : sf::Color::Transparent);

        // render rest of elements
        window.draw(typeQuestion);
        window.draw(movieOption);
        window.draw(showOption);
        window.draw(movieCircle);
        window.draw(showCircle);
        window.draw(movieCircle);
        window.draw(showCircle);
        window.draw(next_button);
        window.draw(next_button_text);
        window.draw(start_over_button);
        window.draw(start_over_text);

        window.display();
    }

    return 0;
}

/*--------------------------------
 * EVERYTHING FOR MOVIE QUESTION WINDOW
 * -------------------------------
*/
int movie_window(int num_recs,string genre) {
    //screen dimensions
    int width = 1600;
    int height = 900;

    //arrays for user choices for movie ratings and movie durations
    string movie_ratings_array[5] = {"All", "7+", "13+", "16+", "18+"};
    string movie_duration_choices[4] = {"<1hr", "1-1.5hrs", "1.5-2hrs", ">2hrs"};

    //create window
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);

    // Load font
    sf::Font textFont;
    textFont.loadFromFile("../files/AmazonEmber_Rg.ttf");

    // Question 1 text (preferd age rating)
    sf::Text age_rating_text;
    age_rating_text.setFont(textFont);
    age_rating_text.setString("What is your prefered age rating?");
    age_rating_text.setCharacterSize(52);
    age_rating_text.setFillColor(sf::Color::White);
    age_rating_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect titleBounds = age_rating_text.getLocalBounds();
    age_rating_text.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    age_rating_text.setPosition(width / 2, 60);

    // Choice boxes (1–5) for ratings
    const int boxCount = 5;
    const float boxWidth = 290;
    const float boxHeight = 180;
    const float spacing = 30;
    const float startX = (width - (boxCount * boxWidth + (boxCount - 1) * spacing)) / 2;
    const float startY = 125;

    //create box choices for movie rating choices
    sf::RectangleShape mov_rating_boxes[boxCount];
    sf::Text mov_ratings[boxCount];
    int selectedIndex = 0;  // Nothing selected at start

    //complete creation of boxes and auto space choices
    for (int i = 0; i < boxCount; ++i) {
        mov_rating_boxes[i].setSize(sf::Vector2f(boxWidth, boxHeight));
        mov_rating_boxes[i].setFillColor(sf::Color::White);
        mov_rating_boxes[i].setPosition(startX + i * (boxWidth + spacing), startY);
        //load text into each box
        mov_ratings[i].setFont(textFont);
        mov_ratings[i].setString((movie_ratings_array[i]));
        mov_ratings[i].setCharacterSize(48);
        mov_ratings[i].setFillColor(sf::Color::Black);
        //spacing
        sf::FloatRect textRect = mov_ratings[i].getLocalBounds();
        mov_ratings[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        mov_ratings[i].setPosition(mov_rating_boxes[i].getPosition().x + boxWidth / 2.0f,
            mov_rating_boxes[i].getPosition().y + boxHeight / 2.0f + 5);
    }

    // Question 2 text (movie duration)
    sf::Text movie_duration_text;
    movie_duration_text.setFont(textFont);
    movie_duration_text.setString("About how long of a movie were you looking for?");
    movie_duration_text.setCharacterSize(52);
    movie_duration_text.setFillColor(sf::Color::White);
    movie_duration_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect durationBounds = movie_duration_text.getLocalBounds();
    movie_duration_text.setOrigin(durationBounds.width / 2, durationBounds.height / 2);
    movie_duration_text.setPosition(800, 350);

    // Choice boxes (1–4) for movie duration
    const int duration_boxCount = 4;
    const float duration_boxWidth = 290;
    const float duration_boxHeight = 180;
    const float duration_spacing = 30;
    const float duration_startX = (width - (duration_boxCount * duration_boxWidth + (duration_boxCount - 1) * duration_spacing)) / 2;
    const float duration_startY = 410;

    //create boxes for movie duration choice
    sf::RectangleShape duration_boxes[duration_boxCount];
    sf::Text mov_duration[duration_boxCount];
    int duration_selectedIndex = 0;  // Nothing selected at start

    //create rest of properties for movie duration choice boxes and auto space them
    for (int i = 0; i < duration_boxCount; ++i) {
        duration_boxes[i].setSize(sf::Vector2f(duration_boxWidth, duration_boxHeight));
        duration_boxes[i].setFillColor(sf::Color::White);
        duration_boxes[i].setPosition(duration_startX + i * (duration_boxWidth + duration_spacing), duration_startY);
        //load text into each box
        mov_duration[i].setFont(textFont);
        mov_duration[i].setString((movie_duration_choices[i]));
        mov_duration[i].setCharacterSize(48);
        mov_duration[i].setFillColor(sf::Color::Black);
        //spacing
        sf::FloatRect textRect = mov_duration[i].getLocalBounds();
        mov_duration[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        mov_duration[i].setPosition(duration_boxes[i].getPosition().x + duration_boxWidth / 2.0f,
            duration_boxes[i].getPosition().y + duration_boxHeight / 2.0f + 5);
    }

    //Question 3 (actors or directors?)
    sf::Text actors_directors_text;
    actors_directors_text.setFont(textFont);
    actors_directors_text.setString("Are there any specific actors or directors you enjoy?");
    actors_directors_text.setCharacterSize(52);
    actors_directors_text.setFillColor(sf::Color::White);
    actors_directors_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect actorBounds = actors_directors_text.getLocalBounds();
    actors_directors_text.setOrigin(actorBounds.width / 2, actorBounds.height / 2);
    actors_directors_text.setPosition(800, 625);

    //create rectangle for user to type into
    sf::RectangleShape type_text(sf::Vector2f(800,50));
    type_text.setPosition(sf::Vector2f(430,700));
    type_text.setScale(sf::Vector2f(1, 1));
    type_text.setFillColor(sf::Color(255,255,255));
    //is box eligible to be typed in?
    bool typing_enabled = false;
    //user typed in info
    string userName = "";
    //user typing text
    sf::Text Usertitle;
    Usertitle.setFont(textFont);
    Usertitle.setString("|");
    Usertitle.setCharacterSize(30);
    Usertitle.setFillColor(sf::Color::Black);
    Usertitle.setStyle(sf::Text::Bold);
    int Usertitlex = type_text.getGlobalBounds().left + 10;
    int Usertitley = type_text.getGlobalBounds().top + 25;
    setText(Usertitle, Usertitlex, Usertitley);

    // Get recommendations button
    sf::CircleShape recommendations_button(100);
    recommendations_button.setPosition(sf::Vector2f(1320,680));
    recommendations_button.setScale(sf::Vector2f(1.2, 1));
    recommendations_button.setFillColor(sf::Color(255,153,0));
    //Get reccomendation button text
    sf::Text recommendations_button_text("            Get\nRecommendations!", textFont);
    recommendations_button_text.setCharacterSize(25);
    recommendations_button_text.setFillColor(sf::Color::White);
    sf::FloatRect recom_Bounds = recommendations_button_text.getLocalBounds();
    recommendations_button_text.setOrigin(recom_Bounds.width / 2, recom_Bounds.height / 2);
    recommendations_button_text.setPosition(1440,765);

    // Start Over button
    sf::CircleShape start_over_button(30);
    start_over_button.setPosition(sf::Vector2f(70,790));
    start_over_button.setScale(sf::Vector2f(4, 1));
    start_over_button.setFillColor(sf::Color(255,153,0));
    //start over button text
    sf::Text start_over_text("Start Over", textFont);
    start_over_text.setCharacterSize(36);
    start_over_text.setFillColor(sf::Color::White);
    sf::FloatRect sobBounds = start_over_text.getLocalBounds();
    start_over_text.setOrigin(sobBounds.width / 2, sobBounds.height / 2);
    start_over_text.setPosition(190,810);

//HANDLES ALL WINDOW EVENTS
    while (window.isOpen()) {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Request for closing the window
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            //Detect mouse click on selections in this window
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                //get mouse position
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                //update selected choice for movie rating
                for (int i = 0; i < boxCount; ++i) {
                    if (mov_rating_boxes[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex = i;
                        break;
                    }
                }
                //update selected choice for movie duration
                for (int i = 0; i < duration_boxCount; ++i) {
                    if (duration_boxes[i].getGlobalBounds().contains(mousePos)) {
                        duration_selectedIndex = i;
                        break;
                    }
                }
                //start over button clicked
                if (start_over_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                    window.close();
                    welcome_window();
                    return 0;
                }

                //update if user is able to type in box based on click
                if (type_text.getGlobalBounds().contains(mousePos)) {
                    typing_enabled = true;
                } else {
                    typing_enabled = false;
                }
                //checks if reccomendation button is pressed to go to reccomendation screen
                if (recommendations_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                    window.close();
                    //updates info parameter based on user inputs
                    vector<string> info;
                    info.push_back("Movie");
                    if (userName == "") {
                        info.push_back("None");
                    } else {
                        info.push_back(userName);
                    }
                    info.push_back(movie_ratings_array[selectedIndex]);
                    info.push_back(movie_duration_choices[duration_selectedIndex]);
                    info.push_back(genre);
                    //go to results window with all required info
                    Results_window(num_recs, info);
                }
            }
            //user typing desired info into text box
            if (typing_enabled && event.type == sf::Event::TextEntered) {
                char enteredChar = static_cast<char>(event.text.unicode);

                // Accept letters, period, apostrophe, dashes, underscores, and space
                if ((enteredChar >= 'A' && enteredChar <= 'Z') ||
                    (enteredChar >= 'a' && enteredChar <= 'z') ||
                    enteredChar == '.' || enteredChar == '\'' || enteredChar == ' ' || enteredChar == '-' || enteredChar == '_') {
                    //limit to 30 characters max
                    if (userName.length() < 30) {
                        userName += enteredChar;
                        Usertitle.setString(userName + "|");
                    }
                    }
            }
            //allow user to delete letters if they want
            else if (typing_enabled && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace && !userName.empty()) {
                    userName = userName.substr(0, userName.length() - 1);
                    Usertitle.setString(userName + "|");
                }
            }
        }

        // Update box colors for movie ratings based on selected index
        for (int i = 0; i < boxCount; ++i) {
            if (i == selectedIndex)
                mov_rating_boxes[i].setFillColor(sf::Color(255, 153, 0));
            else
                mov_rating_boxes[i].setFillColor(sf::Color::White);
        }
        // Update box colors for movie duration based on selected index
        for (int i = 0; i < duration_boxCount; ++i) {
            if (i == duration_selectedIndex)
                duration_boxes[i].setFillColor(sf::Color(255, 153, 0));
            else
                duration_boxes[i].setFillColor(sf::Color::White);
        }

//displays all elements onto the screen
        window.clear(sf::Color(35,47,65));
        //display movie rating choice boxes
        for (int i = 0; i < boxCount; ++i) {
            window.draw(mov_rating_boxes[i]);
            window.draw(mov_ratings[i]);
        }
        //display movie duration boxes
        for (int i = 0; i < duration_boxCount; ++i) {
            window.draw(duration_boxes[i]);
            window.draw(mov_duration[i]);
        }
        //user typing box selection highlights based on selected or not
        if (typing_enabled == true) {
            sf::FloatRect text_bounds = type_text.getGlobalBounds();
            sf::RectangleShape outline;
            outline.setSize(sf::Vector2f(text_bounds.width, text_bounds.height));
            outline.setPosition(text_bounds.left, text_bounds.top);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(4);
            outline.setOutlineColor(sf::Color(255, 153, 0)); // Amazon orange
            window.draw(outline);
        }
        //display the rest of the elements
        window.draw(movie_duration_text);
        window.draw(age_rating_text);
        window.draw(actors_directors_text);
        window.draw(type_text);
        window.draw(Usertitle);
        window.draw(recommendations_button);
        window.draw(recommendations_button_text);
        window.draw(start_over_button);
        window.draw(start_over_text);
        window.display();
    }
}

/*--------------------------------
 * EVERYTHING FOR SHOW QUESTION WINDOW
 * -------------------------------
*/
int show_window(int num_recs,string genre) {
    //dimensions
    int width = 1600;
    int height = 900;

    //arrays for user choices for show ratings and show durations
    string show_ratings_array[7] = {"TV-G", "TV-Y", "TV-Y7", "TV-PG", "TV-14", "TV-MA", "TV-NR"};
    string show_duration_choices[7] = {"1 season", "2 seasons", "3 seasons", "4 seasons", "5 seasons", "6 seasons", ">7 seasons"};

    //render this window
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);

    // Load font
    sf::Font textFont;
    textFont.loadFromFile("../files/AmazonEmber_Rg.ttf");

    // Question 1 text (Prefered age rating for show)
    sf::Text age_rating;
    age_rating.setFont(textFont);
    age_rating.setString("What is your prefered age rating?");
    age_rating.setCharacterSize(52);
    age_rating.setFillColor(sf::Color::White);
    age_rating.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect titleBounds = age_rating.getLocalBounds();
    age_rating.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    age_rating.setPosition(width / 2, 60);

    // Choice boxes (1–7) for show ratings
    const int boxCount = 7;
    const float boxWidth = 190;
    const float boxHeight = 180;
    const float spacing = 30;
    const float startX = (width - (boxCount * boxWidth + (boxCount - 1) * spacing)) / 2;
    const float startY = 125;

    //create box choices for show rating
    sf::RectangleShape boxes[boxCount];
    sf::Text show_ratings[boxCount];
    int selectedIndex = 0;  // Nothing selected at start

    //finish creating boxes for show rating choices and auto space
    for (int i = 0; i < boxCount; ++i) {
        boxes[i].setSize(sf::Vector2f(boxWidth, boxHeight));
        boxes[i].setFillColor(sf::Color::White);
        boxes[i].setPosition(startX + i * (boxWidth + spacing), startY);
        //display text in boxes
        show_ratings[i].setFont(textFont);
        show_ratings[i].setString((show_ratings_array[i]));
        show_ratings[i].setCharacterSize(48);
        show_ratings[i].setFillColor(sf::Color::Black);
        //auto space
        sf::FloatRect textRect = show_ratings[i].getLocalBounds();
        show_ratings[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        show_ratings[i].setPosition(boxes[i].getPosition().x + boxWidth / 2.0f,
            boxes[i].getPosition().y + boxHeight / 2.0f + 5);
    }

    // Question 2 text (show duration)
    sf::Text show_duration;
    show_duration.setFont(textFont);
    show_duration.setString("About how long of a series were you looking for?");
    show_duration.setCharacterSize(52);
    show_duration.setFillColor(sf::Color::White);
    show_duration.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect durationBounds = show_duration.getLocalBounds();
    show_duration.setOrigin(durationBounds.width / 2, durationBounds.height / 2);
    show_duration.setPosition(800, 350);

    // Choice boxes (1–7) for show duration
    const int show_dur_boxCount = 7;
    const float show_dur_boxWidth = 190;
    const float show_dur_boxHeight = 180;
    const float show_dur_spacing = 30;
    const float show_dur_startX = (width - (show_dur_boxCount * show_dur_boxWidth + (show_dur_boxCount - 1) * show_dur_spacing)) / 2;
    const float show_dur_startY = 410;

    //create boxes for show duration choices
    sf::RectangleShape show_dur_boxes[show_dur_boxCount];
    sf::Text show_duration_text[show_dur_boxCount];
    int selectedIndex2 = 0;  // Nothing selected at start

    //finish creating boxes for show rating choices and auto space
    for (int i = 0; i < show_dur_boxCount; ++i) {
        show_dur_boxes[i].setSize(sf::Vector2f(show_dur_boxWidth, show_dur_boxHeight));
        show_dur_boxes[i].setFillColor(sf::Color::White);
        show_dur_boxes[i].setPosition(show_dur_startX + i * (show_dur_boxWidth + show_dur_spacing), show_dur_startY);
        //set text for each box
        show_duration_text[i].setFont(textFont);
        show_duration_text[i].setString((show_duration_choices[i]));
        show_duration_text[i].setCharacterSize(37);
        show_duration_text[i].setFillColor(sf::Color::Black);
        //auto space
        sf::FloatRect textRect = show_duration_text[i].getLocalBounds();
        show_duration_text[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        show_duration_text[i].setPosition(show_dur_boxes[i].getPosition().x + show_dur_boxWidth / 2.0f,
            show_dur_boxes[i].getPosition().y + show_dur_boxHeight / 2.0f + 5);
    }

    //Question 3 (actors or directors?)
    sf::Text actors_directors;
    actors_directors.setFont(textFont);
    actors_directors.setString("Are there any specific actors or directors you enjoy?");
    actors_directors.setCharacterSize(52);
    actors_directors.setFillColor(sf::Color::White);
    actors_directors.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect actorBounds = actors_directors.getLocalBounds();
    actors_directors.setOrigin(actorBounds.width / 2, actorBounds.height / 2);
    actors_directors.setPosition(800, 625);

    //create box for user to type into
    sf::RectangleShape type_text(sf::Vector2f(800,50));
    type_text.setPosition(sf::Vector2f(430,700));
    type_text.setScale(sf::Vector2f(1, 1));
    type_text.setFillColor(sf::Color(255,255,255));

    //checks if text box is able to be typed into
    bool typing_enabled = false;
    //user typed in info
    string userName = "";
    //user typing text
    sf::Text Usertitle;
    Usertitle.setFont(textFont);
    Usertitle.setString("|");
    Usertitle.setCharacterSize(30);
    Usertitle.setFillColor(sf::Color::Black);
    Usertitle.setStyle(sf::Text::Bold);
    int Usertitlex = type_text.getGlobalBounds().left + 10;
    int Usertitley = type_text.getGlobalBounds().top + 25;
    setText(Usertitle, Usertitlex, Usertitley);

    // Get recommendations button
    sf::CircleShape get_recommendations_button(100);
    get_recommendations_button.setPosition(sf::Vector2f(1320,680));
    get_recommendations_button.setScale(sf::Vector2f(1.2, 1));
    get_recommendations_button.setFillColor(sf::Color(255,153,0));
    //get reccomendations button text
    sf::Text recommendations_button_text("            Get\nRecommendations!", textFont);
    recommendations_button_text.setCharacterSize(25);
    recommendations_button_text.setFillColor(sf::Color::White);
    sf::FloatRect recom_Bounds = recommendations_button_text.getLocalBounds();
    recommendations_button_text.setOrigin(recom_Bounds.width / 2, recom_Bounds.height / 2);
    recommendations_button_text.setPosition(1440,765);

    // Start Over button
    sf::CircleShape start_over_button(30);
    start_over_button.setPosition(sf::Vector2f(70,790));
    start_over_button.setScale(sf::Vector2f(4, 1));
    start_over_button.setFillColor(sf::Color(255,153,0));
    //start over button text
    sf::Text start_over_text("Start Over", textFont);
    start_over_text.setCharacterSize(36);
    start_over_text.setFillColor(sf::Color::White);
    sf::FloatRect sobBounds = start_over_text.getLocalBounds();
    start_over_text.setOrigin(sobBounds.width / 2, sobBounds.height / 2);
    start_over_text.setPosition(190,810);

//HANDLES ALL WINDOW EVENTS
    while (window.isOpen()) {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Request for closing the window
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            //Detect mouse click on selections in this window
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                //get mouse position
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                //update selected choice for show rating
                for (int i = 0; i < boxCount; ++i) {
                    if (boxes[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex = i;
                        break;
                    }
                }
                //update selected choice for movie duration
                for (int i = 0; i < show_dur_boxCount; ++i) {
                    if (show_dur_boxes[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex2 = i;
                        break;
                    }
                }
                //start over button clicked
                if (start_over_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                    window.close();
                    welcome_window();
                    return 0;
                }

                //update if user is able to type in box based on click
                if (type_text.getGlobalBounds().contains(mousePos)) {
                    typing_enabled = true;
                } else {
                    typing_enabled = false;
                }
                //checks if recomendation button is pressed to go to recomendation screen
                if (get_recommendations_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                    window.close();
                    //updates info parameter based on user inputs
                    vector<string> info;
                    info.push_back("Show");
                    if (userName == "") {
                        info.push_back("None");
                    } else {
                        info.push_back(userName);
                    }
                    info.push_back(show_ratings_array[selectedIndex]);
                    info.push_back(show_duration_choices[selectedIndex2]);
                    info.push_back(genre);
                    //go to results window with all required info
                    Results_window(num_recs, info);
                }
            }
            //user typing desired info into text box
            if (typing_enabled && event.type == sf::Event::TextEntered) {
                char enteredChar = static_cast<char>(event.text.unicode);

                // Accept letters, period, apostrophe, dashes, underscores, and space
                if ((enteredChar >= 'A' && enteredChar <= 'Z') ||
                    (enteredChar >= 'a' && enteredChar <= 'z') ||
                    enteredChar == '.' || enteredChar == '\'' || enteredChar == ' ' || enteredChar == '-' || enteredChar == '_') {
                    //limit to 30 characters max
                    if (userName.length() < 30) {
                        userName += enteredChar;
                        Usertitle.setString(userName + "|");
                    }
                    }
            }
            //allow user to delete letters if they want
            else if (typing_enabled && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace && !userName.empty()) {
                    userName = userName.substr(0, userName.length() - 1);
                    Usertitle.setString(userName + "|");
                }
            }
        }

        // Update box colors for show ratings based on selected index
        for (int i = 0; i < boxCount; ++i) {
            if (i == selectedIndex)
                boxes[i].setFillColor(sf::Color(255, 153, 0));
            else
                boxes[i].setFillColor(sf::Color::White);
        }
        // Update box colors for show durations based on selected index
        for (int i = 0; i < show_dur_boxCount; ++i) {
            if (i == selectedIndex2)
                show_dur_boxes[i].setFillColor(sf::Color(255, 153, 0));
            else
                show_dur_boxes[i].setFillColor(sf::Color::White);
        }

//displays all elements onto the screen
        window.clear(sf::Color(35,47,65));
        //display show rating choice boxes
        for (int i = 0; i < boxCount; ++i) {
            window.draw(boxes[i]);
            window.draw(show_ratings[i]);
        }
        //display show durations box choice
        for (int i = 0; i < show_dur_boxCount; ++i) {
            window.draw(show_dur_boxes[i]);
            window.draw(show_duration_text[i]);
        }
        //user typing box selection highlights based on selected or not
        if (typing_enabled == true) {
            sf::FloatRect text_bounds = type_text.getGlobalBounds();

            sf::RectangleShape outline;
            outline.setSize(sf::Vector2f(text_bounds.width, text_bounds.height));
            outline.setPosition(text_bounds.left, text_bounds.top);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(4);
            outline.setOutlineColor(sf::Color(255, 153, 0)); // Amazon orange
            window.draw(outline);
        }
        //display the rest of the elements
        window.draw(show_duration);
        window.draw(age_rating);
        window.draw(actors_directors);
        window.draw(type_text);
        window.draw(Usertitle);
        window.draw(get_recommendations_button);
        window.draw(recommendations_button_text);
        window.draw(start_over_button);
        window.draw(start_over_text);
        window.display();
    }
}

/*--------------------------------
 * EVERYTHING FOR RESULTS WINDOW
 * -------------------------------
*/
int Results_window(int num_recs, vector<string> info) {
    //screen dimensions
    int width = 1600;
    int height = 900;

//optional text for testing output of info. Left commented out but may be uncommented out by developer for testing
    // for (size_t i = 0; i < info.size(); ++i) {
    //     cout << (i + 1) << ". " << info[i] << endl;
    // }

    //uses KD tree data structure for calculating reults based on info. Calculates time it takes for calculation
    auto kd_start = chrono::high_resolution_clock::now();
    vector<string> KD_results = global_kdTree->findNearestNeighbors(info, num_recs);
    auto kd_end = chrono::high_resolution_clock::now();
    auto KD_duration = chrono::duration_cast<chrono::nanoseconds>(kd_end - kd_start).count();
    string KD_time_taken = to_string(KD_duration);

    //uses red-black tree data structure for calculating results based on info. Calculates time it takes for calculation
    auto RedBlack_start = chrono::high_resolution_clock::now();
    cout << "Building Red-Black tree...\n";
    global_redblackTree = new RedBlackTree(global_data, info);
    cout << "Red-Black tree built.\n";
    vector<string> RedBlack_results = global_redblackTree->getTopKRecom(num_recs);
    auto RedBlack_end = chrono::high_resolution_clock::now();
    auto RedBlack_duration = chrono::duration_cast<chrono::nanoseconds>(RedBlack_end - RedBlack_start).count();
    string RedBlack_time_taken = to_string(RedBlack_duration);

    //create window
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);

    //Load font
    sf::Font textFont;
    if (!textFont.loadFromFile("../files/AmazonEmber_Rg.ttf")) {
        cerr << "Failed to load font.\n";
        return -1;
    }

    // Titles for results
    //KD results title
    sf::Text kdTitle("K-D Tree Recommendations:", textFont, 36);
    kdTitle.setFillColor(sf::Color(255, 153, 0));
    kdTitle.setPosition(100, 50);
    //RedBlack results title
    sf::Text redblackTitle("Red-Black Tree Recommendations:", textFont, 36);
    redblackTitle.setFillColor(sf::Color(255, 153, 0));
    redblackTitle.setPosition(960, 50);

    // Background white rectangles
    //KD rectangle
    sf::RectangleShape kdBox(sf::Vector2f(550, 700));
    kdBox.setFillColor(sf::Color::White);
    kdBox.setPosition(70, 100);
    //red-black rectangle
    sf::RectangleShape redblackBox(sf::Vector2f(550, 700));
    redblackBox.setFillColor(sf::Color::White);
    redblackBox.setPosition(980, 100);

    // Generate numbered lists
    vector<sf::Text> kdWrappedTexts;
    vector<sf::Text> redblackWrappedTexts;

    //get dimensions to be able to wrap text when it reaches end of box
    float maxTextWidth = kdBox.getSize().x - 40;
    unsigned int charSize = 24;
    float lineSpacing = 25;
    float itemSpacing = 16;

    //dimensions for text to know when to wrap
    float kdX = kdBox.getPosition().x + 20;
    float kdY = kdBox.getPosition().y + 20;
    float redblackX = redblackBox.getPosition().x + 20;
    float redblackY = redblackBox.getPosition().y + 20;

    //display each result for KD tree
    for (int i = 0; i < num_recs; ++i) {
        // Wrap KD text manually
        istringstream iss(KD_results[i]);
        string word, currentLine;
        vector<string> wrappedKD;

        //break lines into multiple lines if too long
        while (iss >> word) {
            string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
            sf::Text temp(testLine, textFont, charSize);
            if (temp.getLocalBounds().width > maxTextWidth) {
                if (!currentLine.empty()) wrappedKD.push_back(currentLine);
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }
        //add final line
        if (!currentLine.empty()) wrappedKD.push_back(currentLine);

        //display all results in wrapped lines for each KD[i]
        for (size_t j = 0; j < wrappedKD.size(); ++j) {
            sf::Text line;
            line.setFont(textFont);
            line.setCharacterSize(charSize);
            line.setFillColor(sf::Color(255, 153, 0));
            line.setString((j == 0 ? to_string(i + 1) + ". " : "    ") + wrappedKD[j]);
            line.setPosition(kdX, kdY);
            kdWrappedTexts.push_back(line);
            kdY += lineSpacing;
        }
        //add space before next recommendation
        kdY += itemSpacing;

        // Wrap Red-black tree text manually
        istringstream issRB(RedBlack_results[i]);
        string wordRB, currentLineRB;
        vector<string> wrappedRB;

        //break line into multiple lines if too long
        while (issRB >> wordRB) {
            string testLine = currentLineRB + (currentLineRB.empty() ? "" : " ") + wordRB;
            sf::Text temp(testLine, textFont, charSize);
            if (temp.getLocalBounds().width > maxTextWidth) {
                if (!currentLineRB.empty()) wrappedRB.push_back(currentLineRB);
                currentLineRB = wordRB;
            } else {
                currentLineRB = testLine;
            }
        }
        //add final line
        if (!currentLineRB.empty()) wrappedRB.push_back(currentLineRB);

        //display all results in wrapped lines for RedBlacktree[i]
        for (size_t j = 0; j < wrappedRB.size(); ++j) {
            sf::Text line;
            line.setFont(textFont);
            line.setCharacterSize(charSize);
            line.setFillColor(sf::Color(255, 153, 0));
            line.setString((j == 0 ? to_string(i + 1) + ". " : "    ") + wrappedRB[j]);
            line.setPosition(redblackX, redblackY);
            redblackWrappedTexts.push_back(line);
            redblackY += lineSpacing;
        }
        //add space before next recommendation
        redblackY += itemSpacing;
    }

// Time labels
    //time label for KD tree
    sf::Text kdTimeLabel("Time taken: " + KD_time_taken + "ns", textFont, 30); //add KD_time_taken to placeholder
    kdTimeLabel.setFillColor(sf::Color(255, 153, 0));
    kdTimeLabel.setPosition(195, 830);

    //time label for red-black tree
    sf::Text RedBlackTimeLabel("Time taken: " + RedBlack_time_taken + "ns", textFont, 30); //add Bplus_time_taken to placeholder
    RedBlackTimeLabel.setFillColor(sf::Color(255, 153, 0));
    RedBlackTimeLabel.setPosition(1120, 830);

    // Start Over button
    sf::CircleShape startOverButton(60);
    startOverButton.setPointCount(90);
    startOverButton.setFillColor(sf::Color(255, 153, 0));
    startOverButton.setScale(2.0f, 1.0f);
    startOverButton.setPosition(width / 2 - 120, 660);

    // Start Over Text
    sf::Text startOverText("Start Over", textFont, 28);
    startOverText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = startOverText.getLocalBounds();
    startOverText.setOrigin(textRect.width / 2, textRect.height / 2);
    startOverText.setPosition(startOverButton.getPosition().x + 120, startOverButton.getPosition().y + 50);

//HANDLES ALL WINDOW EVENTS
    while (window.isOpen()) {
        //event processing
        sf::Event event;
        while (window.pollEvent(event)) {
            //request for closing the window
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            //Detect mouse click on selections in this window
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                //get mouse position
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                //event handling for start over button
                if (startOverButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                    welcome_window();
                    return 0;
                }
            }
        }

//Display all elements on the screen
        window.clear(sf::Color(35, 47, 65));

        window.draw(kdTitle);
        window.draw(redblackTitle);
        window.draw(kdBox);
        window.draw(redblackBox);

        //display results with wrapped text feature on screen
        for (const auto& line : kdWrappedTexts)
            window.draw(line);
        for (const auto& line : redblackWrappedTexts)
            window.draw(line);

        //display rest of the elements
        window.draw(kdTimeLabel);
        window.draw(RedBlackTimeLabel);
        window.draw(startOverButton);
        window.draw(startOverText);
        window.display();
    }
    return 0;
}

//main method to run everything
int main() {

    cout << "Loading CSV data...\n";
    //extract and place CSV data into variable
    global_data = csvExtraction().extractData();
    cout << "Data loaded: " << global_data.size() << " entries\n";

    cout << "Building KD tree...\n";
    //create KD tree before everything else to have to analyze info
    global_kdTree = new balancedKD(global_data);
    cout << "KD tree built.\n";

    //display welcome window
    welcome_window(); // everything starts here

    //delete the trees for memory management
    delete global_kdTree;
    delete global_redblackTree;
}
