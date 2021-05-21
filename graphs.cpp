#include  <SFML/Graphics.hpp>
#include  <iostream>
using namespace sf;

const int width = 830;
const int height = 830;
sf::RenderWindow window(sf::VideoMode(width, height), "");
uint8_t pixels[width * height * 4];
Texture texture;

bool CheckLine(double x, double y)
{
    // y = 3x + 1 - прямая 1;
    // y == -0.6 * x - 2.8 - прямая 2
    //(x - 1.5) * (x - 1,5) + (y - 1, 5) * (y - 1,5) == 2.2 - круг
    // x = -(y + 2.5)^2 + 1 - лежащая парабола
    // y = (x-2.5)^2 - 2 - обычная парабола
    // 2 * abs(x - 3.5) + abs(y - 3.5) = 3 - ромб

    if (abs(x + (y + 2.5) * (y + 2.5) - 1) < 0.3)
        return true;
    if (abs(y - 3 * x - 1) < 0.2)
        return true;
    if (abs(y + 0.6 * x + 2.8) < 0.1)
        return true;
    if (abs(y - (x - 2.5) * (x - 2.5) + 2) < 0.2)
        return true;
    if (abs((x + 2.5) * (x + 2.5) + (y + 2) * (y + 2) - 2.2) < 0.1)
        return true;
    if (x >= 2.5 && x <= 4.5 &&
        ((abs(y - 1.5) < 0.04) ||
        (abs(y + 2.5) < 0.04))) // top & bottom прямоугольника
        return true;
    if (y >= -2.5 && y <= 1.5 &&
        ((abs(x - 2.5) < 0.04) ||
        (abs(x - 4.5) < 0.04))) // left & right прямоугольника
        return true;
    if (abs(2 * abs(x - 3.5) + abs(y - 3.5) - 3) < 0.2)
        return true;
    return false;
}

bool CheckArea(double x, double y)
{
    bool line_ascending = y > 3 * x + 1; // слева
    bool line_descending = y > -0.6 * x - 2.8; // справа
    bool parabola1 = x < -(y + 2.5) * (y + 2.5) + 1; // внутри
    bool circle = (x + 2.5) * (x + 2.5) + (y + 2) * (y + 2) < 2.2; // внутри
    bool parabola2 = y > (x - 2.5)* (x - 2.5) - 2; // внутри
    bool rectangle = x < 4.5 && y < 1.5 && y > -2.5 && x > 2.5; // внутри
    bool rhombus = 2 * abs(x - 3.5) + abs(y - 3.5) < 3; // внутри

    if ((!parabola1 && !line_descending && y > -1) || circle )
        return false;
    if (!line_ascending && x < 0 && !parabola1)
        return true;
    if (circle && line_ascending
        && !parabola2 && !parabola1)
        return true;

    if (!parabola1 && line_ascending && !parabola2)
        return true;

    if (x > 0 && y < 0 && line_descending && !parabola1
        && !parabola2 && !rectangle ||
        (rectangle && parabola2 && !rhombus))
        return true;

    return false;
}

void DrawField() 
{
    for (int ii = 0; ii <= height; ii += 30) {
        sf::RectangleShape line(sf::Vector2f(width - 2, 2));
        line.setFillColor(sf::Color::Black);
        line.setPosition(0, ii);
        window.draw(line);
    }

    for (int ii = 0; ii <= width + 10; ii += 30) {
        sf::RectangleShape line(sf::Vector2f(height, 2));
        line.setFillColor(sf::Color::Black);
        line.setPosition(ii, 0);
        line.rotate(90);
        window.draw(line);
    }

    sf::RectangleShape lineY(sf::Vector2f(height, 6));
    lineY.setFillColor(sf::Color::Black);
    lineY.setPosition((width + 20) / 2, 0);
    lineY.rotate(90);
    window.draw(lineY);

    sf::RectangleShape lineX(sf::Vector2f(width, 6));
    lineX.setFillColor(sf::Color::Black);
    lineX.setPosition(0, (height + 10) / 2);
    window.draw(lineX);
}

void idk()
{
    texture.create(width, height);
    sf::Sprite sprite(texture);
    int centerX = width / 2 + 6;
    int centerY = height / 2 + 6;
    for (register int i = 0; i < width * height * 4; i += 4) {
        double x, y;
        x = (((i) % (width * 4)) / 4 - centerX) / 30.0;
        y = (centerY - ((i) / width / 4)) / 30.0;
        if (CheckArea(x, y)) {
            pixels[i] = 123;
            pixels[i + 1] = 104;
            pixels[i + 2] = 238;
            pixels[i + 3] = 200;
        }
        if (CheckLine(x, y)) {
            pixels[i] = 0;
            pixels[i + 1] = 0;
            pixels[i + 2] = 0;
            pixels[i + 3] = 255;
        }
    }
    texture.update(pixels);
    window.draw(sprite);
}

int main()
{
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(250, 127, 114, 0));
        idk();
        DrawField();
        window.display();
    }
    return 0;
}


