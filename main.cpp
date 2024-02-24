#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

class Bets
{
public:
    int id;
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

class s_line_data
{
public:
    std::vector<sf::RectangleShape> lines;
    s_line_data(float lineWidth,float lineThickness,sf::Color lineColor)
            : s_lineWidth(lineWidth),lineColor(lineColor),lineThickness(lineThickness)
    {
    }
    float s_lineWidth;
    int lineOrder = 0;
    float currentYValue = 0;
    float lineThickness = 0;
    sf::Color lineColor;
    void addLine(sf::RectangleShape line)
    {
        lines.push_back(line);
    }
};

int main(int argc, char * argv[]) {
    std::cout << "Hello, World!" << std::endl;
    sf::RenderWindow window(sf::VideoMode(800,600),"bet");

    sf::View view(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(view);

    float aspectRatio = window.getSize().x / window.getSize().y;

    s_line_data lineData(2,1,sf::Color::White);

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
        nBet.id = i;
        bets.push_back(nBet);
        i++;
    }

    int total = 0;
    float averageTotalWin = 0;

    sf::Font font;
    font.loadFromFile("arial.ttf");

    float lastYValue = 0;
    float totalPercentage = 0;
    float maxWin = 0;
    float totalWin = 0;
    for(auto& e : bets)
    {
        Text betTxt;
        betTxt.SetText(100*e.percentage,e.rate,font);
        texts.push_back(betTxt);
        averageTotalWin += e.rate * e.percentage;
        totalWin += e.rate;
        total++;
        for(auto& b : bets)
        {
            if(e.id != b.id && e.id < b.id)
            {
                averageTotalWin += e.rate * b.rate * e.percentage * b.percentage;
                totalWin += e.rate + b.rate;
                if(maxWin < e.rate * b.rate)
                {
                    maxWin = e.rate * b.rate;
                }
                Text txt;
                txt.SetText(100*e.percentage * b.percentage,e.rate*b.rate,font);
                texts.push_back(txt);
                float lastYValue = lineData.currentYValue;
                float betM = e.rate * b.rate * e.percentage * b.percentage;
                lineData.currentYValue -= betM;
                float degree = atanf(betM / lineData.s_lineWidth) * 180 / M_PI;
                sf::RectangleShape line(sf::Vector2f(sqrt(betM*betM + lineData.s_lineWidth*lineData.s_lineWidth),lineData.lineThickness));
                line.setPosition(sf::Vector2f(lineData.lineOrder * lineData.s_lineWidth,600 - lineData.lineThickness * 3 + lastYValue));
                line.rotate(-degree);
                line.setFillColor(lineData.lineColor);
                lineData.addLine(line);
                lineData.lineOrder++;
                lastYValue = lineData.currentYValue;
                total++;
            }
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
            for(auto& line : lineData.lines)
            {
                window.draw(line);
            }
            sf::Text text;
            text.setCharacterSize(24);
            text.setString("Total bet -> " + std::to_string(total)
                           +"\n""Average total win -> " + std::to_string(averageTotalWin)
                           +"\n""Total win -> " + std::to_string(totalWin)
                           +"\n" + "Max win  -> " + std::to_string(maxWin));
            text.setPosition(0,k*24 + 24);
            text.setFont(font);
            text.setFillColor(sf::Color::White);
            window.draw(text);
            window.display();
        }
    }
    return 0;
}
