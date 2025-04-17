#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <sstream>
#include "Balanced-KD.cpp"
#include "CSV-Extractor.cpp"
using namespace std;

int welcome_window();
int question_window1();
int movie_window(int num_recs,string genre);
int show_window(int num_recs,string genre);
int Results_window(int num_recs, vector<string> info);

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int welcome_window() {
    int width = 1600;
    int height = 900;
    //main window
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);

    //title text
    sf::Font titleFont;
    titleFont.loadFromFile("../files/AmazonEmber_Rg.ttf");
    sf::Text title;
    title.setFont(titleFont);
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
    sf::Text start_button_text("Start Now", titleFont);
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
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);
                    if (start_button.getGlobalBounds().contains(mouse.x,mouse.y)) {
                        cout << "start" << endl;
                        window.close();
                        question_window1();
                    }
                }
            }
        }
        window.clear(sf::Color(35,47,65));
        window.draw(main_imageLogo);
        window.draw(start_button);
        window.draw(start_button_text);
        window.draw(title);
        window.display();
    }
}



void highlightBox(sf::RectangleShape& box) {
    box.setFillColor(sf::Color(255, 153, 0)); // Amazon orange
}



int question_window1() {
    int width = 1600;
    int height = 900;
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);

    string array[12] = {"Kids", "Action and adventure", "Comedy", "Documentary", "Horror", "Drama",
                        "Mystery and thrillers", "Romance", "Fantasy", "Science fiction", "Historical", "Anime"};
    // Load font
    sf::Font titleFont;
    titleFont.loadFromFile("../files/AmazonEmber_Rg.ttf");

    // Question 1 text (how many recommendations)
    sf::Text Recomend_amount;
    Recomend_amount.setFont(titleFont);
    Recomend_amount.setString("How many recommendations would you like?");
    Recomend_amount.setCharacterSize(52);
    Recomend_amount.setFillColor(sf::Color::White);
    Recomend_amount.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect titleBounds = Recomend_amount.getLocalBounds();
    Recomend_amount.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    Recomend_amount.setPosition(width / 2, 100);

    // Choice boxes (1–10)
    const int boxCount = 10;
    const float boxWidth = 100;
    const float boxHeight = 120;
    const float spacing = 30;
    const float startX = (width - (boxCount * boxWidth + (boxCount - 1) * spacing)) / 2;
    const float startY = 160;

    sf::RectangleShape boxes[boxCount];
    sf::Text numbers[boxCount];
    int selectedIndex = 0;  // Nothing selected at start

    for (int i = 0; i < boxCount; ++i) {
        boxes[i].setSize(sf::Vector2f(boxWidth, boxHeight));
        boxes[i].setFillColor(sf::Color::White);
        boxes[i].setPosition(startX + i * (boxWidth + spacing), startY);

        numbers[i].setFont(titleFont);
        numbers[i].setString(std::to_string(i + 1));
        numbers[i].setCharacterSize(48);
        numbers[i].setFillColor(sf::Color::Black);

        sf::FloatRect textRect = numbers[i].getLocalBounds();
        numbers[i].setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        numbers[i].setPosition(
            boxes[i].getPosition().x + boxWidth / 2.0f,
            boxes[i].getPosition().y + boxHeight / 2.0f + 5
        );
    }

    // Question 2 (Genre question)
    sf::Text genreQuestion;
    genreQuestion.setFont(titleFont);
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



    vector<string> imageFiles = {
        "Cat_1.png", "Action_cat.png", "Comedy_cat.png", "Documentary_cat.png", "Horror_cat.png", "Drama_cat.png",
        "Mystery_cat.png", "Romance_cat.png", "Fantasy_cat.png", "SciFi_cat.png", "Historical_cat.png", "Anime_cat.png"
    };

    vector<sf::Texture> textures(imageCount);
    vector<sf::Sprite> sprites(imageCount);

    for (int i = 0; i < imageCount; ++i) {
        string path = "../files/images/" + imageFiles[i];
        if (!textures[i].loadFromFile(path)) {
            cerr << "Failed to load image: " << path << std::endl;
            continue;
        }

        sprites[i].setTexture(textures[i]);
        sprites[i].setScale(0.5f, 0.5f); // scale to 50%

        float x = imageSpacingX + (i % imagesPerRow) * (scaledWidth + imageSpacingX);
        float y = imageStartY + (i / imagesPerRow) * (scaledHeight + imageSpacingY);
        sprites[i].setPosition(x, y);

        std::vector<sf::FloatRect> imageBounds(imageCount);
        imageBounds[i] = sprites[i].getGlobalBounds();
    }
//3rd question (what type of entertainment)
    sf::Text typeQuestion;
    typeQuestion.setFont(titleFont);
    typeQuestion.setString("Are you interested in finding movies or shows?");
    typeQuestion.setCharacterSize(48);
    typeQuestion.setFillColor(sf::Color::White);
    typeQuestion.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect typeBounds = typeQuestion.getLocalBounds();
    typeQuestion.setOrigin(typeBounds.width / 2, typeBounds.height / 2);
    typeQuestion.setPosition(width / 2, imageStartY + 2 * (scaledHeight + imageSpacingY) + 30);

    //option text
    //movie
    sf::Text movieOption, showOption;
    movieOption.setFont(titleFont);
    movieOption.setString("Movies");
    movieOption.setCharacterSize(36);
    movieOption.setFillColor(sf::Color::White);
    movieOption.setPosition(width / 2 - 40, typeQuestion.getPosition().y + 50);
    //show
    showOption.setFont(titleFont);
    showOption.setString("Shows");
    showOption.setCharacterSize(36);
    showOption.setFillColor(sf::Color::White);
    showOption.setPosition(width / 2 - 40, movieOption.getPosition().y + 50);
    //buttons
    sf::CircleShape movieCircle(10);  // radius = 10
    movieCircle.setOutlineColor(sf::Color::White);
    movieCircle.setOutlineThickness(2);
    movieCircle.setFillColor(sf::Color::Transparent);
    movieCircle.setPosition(width / 2 - 80, movieOption.getPosition().y + 10);

    sf::CircleShape showCircle = movieCircle;
    showCircle.setPosition(width / 2 - 80, showOption.getPosition().y + 10);

    int selectedTypeIndex = 0;  // 0 = Movies, 1 = Shows

    // Next button
    sf::CircleShape next_button(30);
    next_button.setPosition(sf::Vector2f(1300,790));
    next_button.setScale(sf::Vector2f(4, 1));
    next_button.setFillColor(sf::Color(255,153,0));
    //next button text
    sf::Text next_button_text("Continue", titleFont);
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
    sf::Text start_over_text("Start Over", titleFont);
    start_over_text.setCharacterSize(36);
    start_over_text.setFillColor(sf::Color::White);
    sf::FloatRect sobBounds = start_over_text.getLocalBounds();
    start_over_text.setOrigin(sobBounds.width / 2, sobBounds.height / 2);
    start_over_text.setPosition(190,810);

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

        // Detect mouse click on genre images
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // Number box selection
            for (int i = 0; i < boxCount; ++i) {
                if (boxes[i].getGlobalBounds().contains(mousePos)) {
                    selectedIndex = i;
                }
            }

            // Genre image selection
            for (int i = 0; i < imageCount; ++i) {
                if (sprites[i].getGlobalBounds().contains(mousePos)) {
                    selectedGenreIndex = i; // ONLY update on click
                }
            }
            //bubble choice selection
            if (movieCircle.getGlobalBounds().contains(mousePos)) {
                selectedTypeIndex = 0;
            } else if (showCircle.getGlobalBounds().contains(mousePos)) {
                selectedTypeIndex = 1;
            }

            if (next_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                cout << "start" << endl;
                if (selectedTypeIndex == 0) {
                    window.close();
                    movie_window((selectedIndex + 1), array[selectedGenreIndex]);
                } else {
                    window.close();
                    show_window((selectedIndex + 1), array[selectedGenreIndex]);
                }
                return 0;
            }
            if (start_over_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                cout << "redo" << endl;
                window.close();
                welcome_window();
                return 0;
            }
        }


        window.clear(sf::Color(35, 47, 65)); // Amazon gray
        window.draw(Recomend_amount);

        for (int i = 0; i < boxCount; ++i) {
            window.draw(boxes[i]);
            window.draw(numbers[i]);
        }


        //Question 2 Event handeling
        window.draw(genreQuestion);

        for (int i = 0; i < imageCount; ++i) {
            window.draw(sprites[i]);
        }
        // Draw selection outline if a genre is selected
        if (selectedGenreIndex != -1) {
            sf::FloatRect bounds = sprites[selectedGenreIndex].getGlobalBounds();

            sf::RectangleShape outline;
            outline.setSize(sf::Vector2f(bounds.width, bounds.height));
            outline.setPosition(bounds.left, bounds.top);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(4);
            outline.setOutlineColor(sf::Color(255, 153, 0)); // Amazon orange

            window.draw(outline);
        }

        // Apply orange fill if selected, transparent otherwise
        movieCircle.setFillColor(selectedTypeIndex == 0 ? sf::Color(255, 153, 0) : sf::Color::Transparent);
        showCircle.setFillColor(selectedTypeIndex == 1 ? sf::Color(255, 153, 0) : sf::Color::Transparent);

        // Then draw them
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


int movie_window(int num_recs,string genre) {
    int width = 1600;
    int height = 900;
    string movie_ratings[5] = {"All", "7+", "13+", "16+", "18+"};
    string movie_duration_choices[4] = {"<1hr", "1-1.5hrs", "1.5-2hrs", ">2hrs"};
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);
    // Load font
    sf::Font titleFont;
    titleFont.loadFromFile("../files/AmazonEmber_Rg.ttf");

    // Question 1 text (how many recommendations)
    sf::Text age_rating;
    age_rating.setFont(titleFont);
    age_rating.setString("What is your preffered age rating?");
    age_rating.setCharacterSize(52);
    age_rating.setFillColor(sf::Color::White);
    age_rating.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect titleBounds = age_rating.getLocalBounds();
    age_rating.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    age_rating.setPosition(width / 2, 60);

    // Choice boxes (1–10)
    const int boxCount = 5;
    const float boxWidth = 290;
    const float boxHeight = 180;
    const float spacing = 30;
    const float startX = (width - (boxCount * boxWidth + (boxCount - 1) * spacing)) / 2;
    const float startY = 125;

    sf::RectangleShape boxes[boxCount];
    sf::Text mov_ratings[boxCount];
    int selectedIndex = 0;  // Nothing selected at start

    for (int i = 0; i < boxCount; ++i) {
        boxes[i].setSize(sf::Vector2f(boxWidth, boxHeight));
        boxes[i].setFillColor(sf::Color::White);
        boxes[i].setPosition(startX + i * (boxWidth + spacing), startY);

        mov_ratings[i].setFont(titleFont);
        mov_ratings[i].setString((movie_ratings[i]));
        mov_ratings[i].setCharacterSize(48);
        mov_ratings[i].setFillColor(sf::Color::Black);

        sf::FloatRect textRect = mov_ratings[i].getLocalBounds();
        mov_ratings[i].setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        mov_ratings[i].setPosition(
            boxes[i].getPosition().x + boxWidth / 2.0f,
            boxes[i].getPosition().y + boxHeight / 2.0f + 5
        );
    }

    // Question 2 text (movie duration)
    sf::Text movie_duration;
    movie_duration.setFont(titleFont);
    movie_duration.setString("About how long of a movie were you looking for?");
    movie_duration.setCharacterSize(52);
    movie_duration.setFillColor(sf::Color::White);
    movie_duration.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect durationBounds = movie_duration.getLocalBounds();
    movie_duration.setOrigin(durationBounds.width / 2, durationBounds.height / 2);
    movie_duration.setPosition(800, 350);

    // Choice boxes (1–10)
    const int boxCount2 = 4;
    const float boxWidth2 = 290;
    const float boxHeight2 = 180;
    const float spacing2 = 30;
    const float startX2 = (width - (boxCount2 * boxWidth2 + (boxCount2 - 1) * spacing2)) / 2;
    const float startY2 = 410;

    sf::RectangleShape boxes2[boxCount2];
    sf::Text mov_duration[boxCount2];
    int selectedIndex2 = 0;  // Nothing selected at start

    for (int i = 0; i < boxCount2; ++i) {
        boxes2[i].setSize(sf::Vector2f(boxWidth2, boxHeight2));
        boxes2[i].setFillColor(sf::Color::White);
        boxes2[i].setPosition(startX2 + i * (boxWidth2 + spacing2), startY2);

        mov_duration[i].setFont(titleFont);
        mov_duration[i].setString((movie_duration_choices[i]));
        mov_duration[i].setCharacterSize(48);
        mov_duration[i].setFillColor(sf::Color::Black);

        sf::FloatRect textRect = mov_duration[i].getLocalBounds();
        mov_duration[i].setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        mov_duration[i].setPosition(
            boxes2[i].getPosition().x + boxWidth2 / 2.0f,
            boxes2[i].getPosition().y + boxHeight2 / 2.0f + 5
        );
    }

    //Question 3 (actors or directors?)
    sf::Text actors_directors;
    actors_directors.setFont(titleFont);
    actors_directors.setString("Are there any specific actors or directors you enjoy?");
    actors_directors.setCharacterSize(52);
    actors_directors.setFillColor(sf::Color::White);
    actors_directors.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect actorBounds = actors_directors.getLocalBounds();
    actors_directors.setOrigin(actorBounds.width / 2, actorBounds.height / 2);
    actors_directors.setPosition(800, 625);

    sf::RectangleShape type_text(sf::Vector2f(800,50));
    type_text.setPosition(sf::Vector2f(430,700));
    type_text.setScale(sf::Vector2f(1, 1));
    type_text.setFillColor(sf::Color(255,255,255));

    bool typing_enabled = false;
    string userName = "";
    //user typing
    sf::Text Usertitle;
    Usertitle.setFont(titleFont);
    Usertitle.setString("|");
    Usertitle.setCharacterSize(30);
    Usertitle.setFillColor(sf::Color::Black);
    Usertitle.setStyle(sf::Text::Bold);
    int Usertitlex = type_text.getGlobalBounds().left + 10;
    int Usertitley = type_text.getGlobalBounds().top + 25;
    setText(Usertitle, Usertitlex, Usertitley);

    // Get recommendations button
    sf::CircleShape next_button(100);
    next_button.setPosition(sf::Vector2f(1320,680));
    next_button.setScale(sf::Vector2f(1.2, 1));
    next_button.setFillColor(sf::Color(255,153,0));
    //next button text
    sf::Text next_button_text("            Get\nRecommendations!", titleFont);
    next_button_text.setCharacterSize(25);
    next_button_text.setFillColor(sf::Color::White);
    sf::FloatRect sbBounds = next_button_text.getLocalBounds();
    next_button_text.setOrigin(sbBounds.width / 2, sbBounds.height / 2);
    next_button_text.setPosition(1440,765);

    // Start Over button
    sf::CircleShape start_over_button(30);
    start_over_button.setPosition(sf::Vector2f(70,790));
    start_over_button.setScale(sf::Vector2f(4, 1));
    start_over_button.setFillColor(sf::Color(255,153,0));
    //start over button text
    sf::Text start_over_text("Start Over", titleFont);
    start_over_text.setCharacterSize(36);
    start_over_text.setFillColor(sf::Color::White);
    sf::FloatRect sobBounds = start_over_text.getLocalBounds();
    start_over_text.setOrigin(sobBounds.width / 2, sobBounds.height / 2);
    start_over_text.setPosition(190,810);

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
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                for (int i = 0; i < boxCount; ++i) {
                    if (boxes[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex = i;
                        break;
                    }
                }
                for (int i = 0; i < boxCount2; ++i) {
                    if (boxes2[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex2 = i;
                        break;
                    }
                }
                if (start_over_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                    cout << "redo" << endl;
                    window.close();
                    welcome_window();
                    return 0;
                }

                if (type_text.getGlobalBounds().contains(mousePos)) {
                    typing_enabled = true;
                } else {
                    typing_enabled = false;
                }
                if (next_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                    cout << "start" << endl;
                    window.close();
                    vector<string> info;
                    info.push_back("Movie");
                    if (userName == "") {
                        info.push_back("None");
                    } else {
                        info.push_back(userName);
                    }
                    info.push_back(movie_ratings[selectedIndex]);
                    info.push_back(movie_duration_choices[selectedIndex2]);
                    info.push_back(genre);
                    Results_window(num_recs, info);
                }
            }
            if (typing_enabled && event.type == sf::Event::TextEntered) {
                char enteredChar = static_cast<char>(event.text.unicode);

                // Accept letters, period, apostrophe, and space
                if ((enteredChar >= 'A' && enteredChar <= 'Z') ||
                    (enteredChar >= 'a' && enteredChar <= 'z') ||
                    enteredChar == '.' || enteredChar == '\'' || enteredChar == ' ' || enteredChar == '-' || enteredChar == '_') {

                    if (userName.length() < 30) {
                        userName += enteredChar;
                        Usertitle.setString(userName + "|");
                    }
                    }
            }
            else if (typing_enabled && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace && !userName.empty()) {
                    userName = userName.substr(0, userName.length() - 1);
                    Usertitle.setString(userName + "|");
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
        for (int i = 0; i < boxCount2; ++i) {
            if (i == selectedIndex2)
                boxes2[i].setFillColor(sf::Color(255, 153, 0));
            else
                boxes2[i].setFillColor(sf::Color::White);
        }

        window.clear(sf::Color(35,47,65));

        for (int i = 0; i < boxCount; ++i) {
            window.draw(boxes[i]);
            window.draw(mov_ratings[i]);
        }
        for (int i = 0; i < boxCount2; ++i) {
            window.draw(boxes2[i]);
            window.draw(mov_duration[i]);
        }
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
        window.draw(movie_duration);
        window.draw(age_rating);
        window.draw(actors_directors);
        window.draw(type_text);
        window.draw(Usertitle);
        window.draw(next_button);
        window.draw(next_button_text);
        window.draw(start_over_button);
        window.draw(start_over_text);
        window.display();
    }
}


int show_window(int num_recs,string genre) {
    int width = 1600;
    int height = 900;
    string movie_ratings[7] = {"TV-G", "TV-Y", "TV-Y7", "TV-PG", "TV-14", "TV-MA", "TV-NR"};
    string movie_duration_choices[7] = {"1 season", "2 seasons", "3 seasons", "4 seasons", "5 seasons", "6 seasons", ">7 seasons"};
    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);
    // Load font
    sf::Font titleFont;
    titleFont.loadFromFile("../files/AmazonEmber_Rg.ttf");

    // Question 1 text (how many recommendations)
    sf::Text age_rating;
    age_rating.setFont(titleFont);
    age_rating.setString("What is your preffered age rating?");
    age_rating.setCharacterSize(52);
    age_rating.setFillColor(sf::Color::White);
    age_rating.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect titleBounds = age_rating.getLocalBounds();
    age_rating.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    age_rating.setPosition(width / 2, 60);

    // Choice boxes (1–10)
    const int boxCount = 7;
    const float boxWidth = 190;
    const float boxHeight = 180;
    const float spacing = 30;
    const float startX = (width - (boxCount * boxWidth + (boxCount - 1) * spacing)) / 2;
    const float startY = 125;

    sf::RectangleShape boxes[boxCount];
    sf::Text mov_ratings[boxCount];
    int selectedIndex = 0;  // Nothing selected at start

    for (int i = 0; i < boxCount; ++i) {
        boxes[i].setSize(sf::Vector2f(boxWidth, boxHeight));
        boxes[i].setFillColor(sf::Color::White);
        boxes[i].setPosition(startX + i * (boxWidth + spacing), startY);

        mov_ratings[i].setFont(titleFont);
        mov_ratings[i].setString((movie_ratings[i]));
        mov_ratings[i].setCharacterSize(48);
        mov_ratings[i].setFillColor(sf::Color::Black);

        sf::FloatRect textRect = mov_ratings[i].getLocalBounds();
        mov_ratings[i].setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        mov_ratings[i].setPosition(
            boxes[i].getPosition().x + boxWidth / 2.0f,
            boxes[i].getPosition().y + boxHeight / 2.0f + 5
        );
    }

    // Question 2 text (movie duration)
    sf::Text movie_duration;
    movie_duration.setFont(titleFont);
    movie_duration.setString("About how long of a series were you looking for?");
    movie_duration.setCharacterSize(52);
    movie_duration.setFillColor(sf::Color::White);
    movie_duration.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect durationBounds = movie_duration.getLocalBounds();
    movie_duration.setOrigin(durationBounds.width / 2, durationBounds.height / 2);
    movie_duration.setPosition(800, 350);

    // Choice boxes (1–10)
    const int boxCount2 = 7;
    const float boxWidth2 = 190;
    const float boxHeight2 = 180;
    const float spacing2 = 30;
    const float startX2 = (width - (boxCount2 * boxWidth2 + (boxCount2 - 1) * spacing2)) / 2;
    const float startY2 = 410;

    sf::RectangleShape boxes2[boxCount2];
    sf::Text mov_duration[boxCount2];
    int selectedIndex2 = 0;  // Nothing selected at start

    for (int i = 0; i < boxCount2; ++i) {
        boxes2[i].setSize(sf::Vector2f(boxWidth2, boxHeight2));
        boxes2[i].setFillColor(sf::Color::White);
        boxes2[i].setPosition(startX2 + i * (boxWidth2 + spacing2), startY2);

        mov_duration[i].setFont(titleFont);
        mov_duration[i].setString((movie_duration_choices[i]));
        mov_duration[i].setCharacterSize(37);
        mov_duration[i].setFillColor(sf::Color::Black);

        sf::FloatRect textRect = mov_duration[i].getLocalBounds();
        mov_duration[i].setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        mov_duration[i].setPosition(
            boxes2[i].getPosition().x + boxWidth2 / 2.0f,
            boxes2[i].getPosition().y + boxHeight2 / 2.0f + 5
        );
    }

    //Question 3 (actors or directors?)
    sf::Text actors_directors;
    actors_directors.setFont(titleFont);
    actors_directors.setString("Are there any specific actors or directors you enjoy?");
    actors_directors.setCharacterSize(52);
    actors_directors.setFillColor(sf::Color::White);
    actors_directors.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect actorBounds = actors_directors.getLocalBounds();
    actors_directors.setOrigin(actorBounds.width / 2, actorBounds.height / 2);
    actors_directors.setPosition(800, 625);

    sf::RectangleShape type_text(sf::Vector2f(800,50));
    type_text.setPosition(sf::Vector2f(430,700));
    type_text.setScale(sf::Vector2f(1, 1));
    type_text.setFillColor(sf::Color(255,255,255));

    bool typing_enabled = false;
    string userName = "";
    //user typing
    sf::Text Usertitle;
    Usertitle.setFont(titleFont);
    Usertitle.setString("|");
    Usertitle.setCharacterSize(30);
    Usertitle.setFillColor(sf::Color::Black);
    Usertitle.setStyle(sf::Text::Bold);
    int Usertitlex = type_text.getGlobalBounds().left + 10;
    int Usertitley = type_text.getGlobalBounds().top + 25;
    setText(Usertitle, Usertitlex, Usertitley);

    // Get recommendations button
    sf::CircleShape next_button(100);
    next_button.setPosition(sf::Vector2f(1320,680));
    next_button.setScale(sf::Vector2f(1.2, 1));
    next_button.setFillColor(sf::Color(255,153,0));
    //next button text
    sf::Text next_button_text("            Get\nRecommendations!", titleFont);
    next_button_text.setCharacterSize(25);
    next_button_text.setFillColor(sf::Color::White);
    sf::FloatRect sbBounds = next_button_text.getLocalBounds();
    next_button_text.setOrigin(sbBounds.width / 2, sbBounds.height / 2);
    next_button_text.setPosition(1440,765);

    // Start Over button
    sf::CircleShape start_over_button(30);
    start_over_button.setPosition(sf::Vector2f(70,790));
    start_over_button.setScale(sf::Vector2f(4, 1));
    start_over_button.setFillColor(sf::Color(255,153,0));
    //start over button text
    sf::Text start_over_text("Start Over", titleFont);
    start_over_text.setCharacterSize(36);
    start_over_text.setFillColor(sf::Color::White);
    sf::FloatRect sobBounds = start_over_text.getLocalBounds();
    start_over_text.setOrigin(sobBounds.width / 2, sobBounds.height / 2);
    start_over_text.setPosition(190,810);

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
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                for (int i = 0; i < boxCount; ++i) {
                    if (boxes[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex = i;
                        break;
                    }
                }
                for (int i = 0; i < boxCount2; ++i) {
                    if (boxes2[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex2 = i;
                        break;
                    }
                }
                if (start_over_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                    cout << "redo" << endl;
                    window.close();
                    welcome_window();
                    return 0;
                }

                if (type_text.getGlobalBounds().contains(mousePos)) {
                    typing_enabled = true;
                } else {
                    typing_enabled = false;
                }
                if (next_button.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                    cout << "start" << endl;
                    window.close();
                    vector<string> info;
                    info.push_back("Show");
                    if (userName == "") {
                        info.push_back("None");
                    } else {
                        info.push_back(userName);
                    }
                    info.push_back(movie_ratings[selectedIndex]);
                    info.push_back(movie_duration_choices[selectedIndex2]);
                    info.push_back(genre);
                    Results_window(num_recs, info);
                }
            }
            if (typing_enabled && event.type == sf::Event::TextEntered) {
                char enteredChar = static_cast<char>(event.text.unicode);

                // Accept letters, period, apostrophe, and space
                if ((enteredChar >= 'A' && enteredChar <= 'Z') ||
                    (enteredChar >= 'a' && enteredChar <= 'z') ||
                    enteredChar == '.' || enteredChar == '\'' || enteredChar == ' ' || enteredChar == '-' || enteredChar == '_') {

                    if (userName.length() < 30) {
                        userName += enteredChar;
                        Usertitle.setString(userName + "|");
                    }
                    }
            }
            else if (typing_enabled && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace && !userName.empty()) {
                    userName = userName.substr(0, userName.length() - 1);
                    Usertitle.setString(userName + "|");
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
        for (int i = 0; i < boxCount2; ++i) {
            if (i == selectedIndex2)
                boxes2[i].setFillColor(sf::Color(255, 153, 0));
            else
                boxes2[i].setFillColor(sf::Color::White);
        }

        window.clear(sf::Color(35,47,65));

        for (int i = 0; i < boxCount; ++i) {
            window.draw(boxes[i]);
            window.draw(mov_ratings[i]);
        }
        for (int i = 0; i < boxCount2; ++i) {
            window.draw(boxes2[i]);
            window.draw(mov_duration[i]);
        }
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
        window.draw(movie_duration);
        window.draw(age_rating);
        window.draw(actors_directors);
        window.draw(type_text);
        window.draw(Usertitle);
        window.draw(next_button);
        window.draw(next_button_text);
        window.draw(start_over_button);
        window.draw(start_over_text);
        window.display();
    }
}


int Results_window(int num_recs, vector<string> info) {
    int width = 1600;
    int height = 900;

    cout <<"working before KD tree" << endl;
    for (size_t i = 0; i < info.size(); ++i) {
        std::cout << (i + 1) << ". " << info[i] << std::endl;
    }

//stack overflow
    // vector<vector<vector<string>>> data;
    // data = csvExtraction().extractData();
    // balancedKD kdTree(data);
    // vector<string> neighbors = kdTree.findNearestNeighbors(info, num_recs);
    // vector<string> KD_results = kdTree.findNearestNeighbors(info, num_recs);

    sf::RenderWindow window(sf::VideoMode(width, height), "Amazon Catalog Recommender", sf::Style::Close);

    cout << "working after KD tree" << endl;

    sf::Font font;
    if (!font.loadFromFile("../files/AmazonEmber_Rg.ttf")) {
        std::cerr << "Failed to load font.\n";
        return -1;
    }

    // Titles
    sf::Text kdTitle("K-D Tree Recommendations:", font, 36);
    kdTitle.setFillColor(sf::Color(255, 153, 0));
    kdTitle.setPosition(100, 50);

    sf::Text bplusTitle("B+ Tree Recommendations:", font, 36);
    bplusTitle.setFillColor(sf::Color(255, 153, 0));
    bplusTitle.setPosition(1010, 50);

    // Background white rectangles
    sf::RectangleShape kdBox(sf::Vector2f(550, 700));
    kdBox.setFillColor(sf::Color::White);
    kdBox.setPosition(70, 100);

    sf::RectangleShape bplusBox(sf::Vector2f(550, 700));
    bplusBox.setFillColor(sf::Color::White);
    bplusBox.setPosition(980, 100);

    // Generate numbered lists
    std::vector<sf::Text> kdWrappedTexts;
    std::vector<sf::Text> bplusWrappedTexts;

    float maxTextWidth = kdBox.getSize().x - 40;
    unsigned int charSize = 24;
    float lineSpacing = 25;
    float itemSpacing = 16;

    float kdX = kdBox.getPosition().x + 20;
    float kdY = kdBox.getPosition().y + 20;
    float bplusX = bplusBox.getPosition().x + 20;
    float bplusY = bplusBox.getPosition().y + 20;

    for (int i = 0; i < num_recs; ++i) {
        // Wrap KD text manually
        std::istringstream iss("placeholder"); //add KD_results[i] later when ready
        std::string word, currentLine;
        std::vector<std::string> wrappedKD;

        while (iss >> word) {
            std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
            sf::Text temp(testLine, font, charSize);
            if (temp.getLocalBounds().width > maxTextWidth) {
                if (!currentLine.empty()) wrappedKD.push_back(currentLine);
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }
        if (!currentLine.empty()) wrappedKD.push_back(currentLine);

        for (size_t j = 0; j < wrappedKD.size(); ++j) {
            sf::Text line;
            line.setFont(font);
            line.setCharacterSize(charSize);

            line.setFillColor(sf::Color(255, 153, 0));
            line.setString((j == 0 ? std::to_string(i + 1) + ". " : "    ") + wrappedKD[j]);
            line.setPosition(kdX, kdY);
            kdWrappedTexts.push_back(line);
            kdY += lineSpacing;
        }
        kdY += itemSpacing;

        // Wrap B+ text manually
        std::istringstream issB("Yoga Therapy For Back Pain, Neck Pain & Stress Relief - Lindsey Samper"); //add Bplus_results[i] later when finished
        std::string wordB, currentLineB;
        std::vector<std::string> wrappedB;

        while (issB >> wordB) {
            std::string testLine = currentLineB + (currentLineB.empty() ? "" : " ") + wordB;
            sf::Text temp(testLine, font, charSize);
            if (temp.getLocalBounds().width > maxTextWidth) {
                if (!currentLineB.empty()) wrappedB.push_back(currentLineB);
                currentLineB = wordB;
            } else {
                currentLineB = testLine;
            }
        }
        if (!currentLineB.empty()) wrappedB.push_back(currentLineB);

        for (size_t j = 0; j < wrappedB.size(); ++j) {
            sf::Text line;
            line.setFont(font);
            line.setCharacterSize(charSize);

            line.setFillColor(sf::Color(255, 153, 0));
            line.setString((j == 0 ? std::to_string(i + 1) + ". " : "    ") + wrappedB[j]);
            line.setPosition(bplusX, bplusY);
            bplusWrappedTexts.push_back(line);
            bplusY += lineSpacing;
        }
        bplusY += itemSpacing;
    }

    // Time labels
    sf::Text kdTimeLabel("Time taken: 00:00", font, 30); //add KD_time_taken to placeholder
    kdTimeLabel.setFillColor(sf::Color(255, 153, 0));
    kdTimeLabel.setPosition(205, 830);

    sf::Text bplusTimeLabel("Time taken: 00:00", font, 30); //add Bplus_time_taken to placeholder
    bplusTimeLabel.setFillColor(sf::Color(255, 153, 0));
    bplusTimeLabel.setPosition(1130, 830);

    // Oval Start Over button using CircleShape
    sf::CircleShape startOverButton(60);  // radius = 60 (will be stretched)
    startOverButton.setPointCount(90);    // smoother circle
    startOverButton.setFillColor(sf::Color(255, 153, 0));
    startOverButton.setScale(2.0f, 1.0f); // stretch horizontally to make it oval
    startOverButton.setPosition(width / 2 - 120, 660); // adjusted for scale


    sf::Text startOverText("Start Over", font, 28);
    startOverText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = startOverText.getLocalBounds();
    startOverText.setOrigin(textRect.width / 2, textRect.height / 2);
    startOverText.setPosition(startOverButton.getPosition().x + 120, startOverButton.getPosition().y + 50);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (startOverButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                    welcome_window();  // assuming this exists
                    return 0;
                }
            }
        }

        window.clear(sf::Color(35, 47, 65));

        window.draw(kdTitle);
        window.draw(bplusTitle);
        window.draw(kdBox);
        window.draw(bplusBox);

        for (const auto& line : kdWrappedTexts)
            window.draw(line);
        for (const auto& line : bplusWrappedTexts)
            window.draw(line);


        window.draw(kdTimeLabel);
        window.draw(bplusTimeLabel);
        window.draw(startOverButton);
        window.draw(startOverText);

        window.display();
    }

    return 0;
}




int main() {
    welcome_window();
}
