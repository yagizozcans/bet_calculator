#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

class Bets
{
public:
    float rate;
    float percentage;
};

class Text
{
public:
    sf::Text text;
    void SetText(float betPercentage,float betValue,sf::Font& font)
    {
        text.setCharacterSize(24);
        text.setString("Bet percentage ->  " + std::to_string(betPercentage)
        + " | " + "Bet value -> " + std::to_string(betValue));
        text.setFont(font);
        text.setFillColor(sf::Color::White);
    }
};

int main(int argc, char * argv[]) {
    std::cout << "Hello, World!" << std::endl;
    sf::RenderWindow window(sf::VideoMode(800,600),"bet");

    sf::View view(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(view);

    float aspectRatio = window.getSize().x / window.getSize().y;

    std::vector<Bets> bets;
    std::vector<Text> texts;
    std::ifstream fin("bets.txt");

    float token = 0;
    int i = 0;

    while(fin >> token)
    {
        Bets nBet;
        nBet.rate = token;
        fin>>token;
        nBet.percentage = token;
        bets.push_back(nBet);
        i++;
    }

    int total = 0;
    float averageTotalWin = 0;

    sf::Font font;
    font.loadFromFile("arial.ttf");

    for(auto& e : bets)
    {
        for(auto& b : bets)
        {
            averageTotalWin += e.rate * b.rate * e.percentage * b.percentage;
            total++;
            Text txt;
            txt.SetText(e.rate * b.rate * e.percentage * b.percentage,e.rate*b.rate,font);
            texts.push_back(txt);
        }
    }

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type == sf::Event::MouseWheelScrolled)
            {
                std::cout<<event.mouseWheelScroll.delta<<std::endl;
                view.setSize(view.getSize().x + event.mouseWheelScroll.delta * aspectRatio,
                             view.getSize().y + event.mouseWheelScroll.delta);
                window.setView(view);
            }
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.scancode == sf::Keyboard::W)
                {
                    view.move(0,-100);
                }
                if(event.key.scancode == sf::Keyboard::S)
                {
                    view.move(0,100);
                }
                if(event.key.scancode == sf::Keyboard::A)
                {
                    view.move(-100,0);
                }
                if(event.key.scancode == sf::Keyboard::D)
                {
                    view.move(100,0);
                }
                window.setView(view);
            }
            window.clear(sf::Color::Black);
            int k = 0;
            for(auto& t : texts)
            {
                t.text.setPosition(0,k*24);
                window.draw(t.text);
                k++;
            }
            sf::Text text;
            float totalGain = averageTotalWin * total;
            text.setCharacterSize(24);
            text.setString("Total bet -> " + std::to_string(total)
                           +"\n""Average total win -> " + std::to_string(totalGain)
                           +"\n"+"Percentage of bet -> " + std::to_string(averageTotalWin)
                           +"\n" + "Gain per one bet -> " + std::to_string((totalGain - total)/total));
            text.setPosition(0,k*24 + 24);
            text.setFont(font);
            text.setFillColor(sf::Color::White);
            window.draw(text);
            window.display();
        }
    }
    return 0;
}
