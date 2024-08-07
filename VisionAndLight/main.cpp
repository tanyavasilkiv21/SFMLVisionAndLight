#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class ShapeEntity
{
public:
    sf::ConvexShape shape;

    ShapeEntity(const std::vector<sf::Vector2f>& points, sf::Vector2f pos) {
        shape.setPointCount(points.size());
        for (size_t i = 0; i < points.size(); ++i) {
            shape.setPoint(i, points[i]);
        }
        shape.setPosition(pos);
        shape.setOutlineThickness(1);
        shape.setOutlineColor(sf::Color::Black);
        shape.setFillColor(sf::Color::Transparent);
    }
};

struct Intersect
{
    bool result;
    sf::Vector2f pos;
};

float crossProduct(sf::Vector2f a, sf::Vector2f b)
{
    return ((a.x * b.y) - (a.y * b.x));
}
Intersect LineIntersect(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
{
    auto r = b - a;
    auto s = d - c;
    float rxs = crossProduct(r, s);
    auto cma = c - a;
    float t = (crossProduct(cma, s) / rxs);
    float u = (crossProduct(cma, r) / rxs);

    std::cout << t << "   " << u << std::endl;
    if (t <= 1 && t >= 0 && u <= 1 && u >= 0)
    {
        return { true, sf::Vector2f(a.x + t * r.x, a.y + t * r.y) };
    }
    return  { false, sf::Vector2f(0,0) };
}

void drawLine(sf::Vector2f p1, sf::Vector2f p2, sf::RenderWindow& window, sf::Color color)
{
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = p1;
    line[0].color = color;
    line[1].position = p2;
    line[1].color = color;
    window.draw(line);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Vision and Light");
    window.setFramerateLimit(60);
    std::vector<ShapeEntity> shapes;
    shapes.emplace_back(std::vector<sf::Vector2f>{{0, 0}, { 50, 0 }, { 50, 50 }, { 0, 50 }}, sf::Vector2f(100, 100));
    shapes.emplace_back(std::vector<sf::Vector2f>{{0, 0}, { 30, 0 }, { 30, 30 }, { 0, 30 }}, sf::Vector2f(200, 200));
    shapes.emplace_back(std::vector<sf::Vector2f>{{0, 0}, { 70, 0 }, { 70, 70 }, { 0, 70 }}, sf::Vector2f(300, 300));
    shapes.emplace_back(std::vector<sf::Vector2f>{{0, 0}, { 60, 0 }, { 30, 50 }}, sf::Vector2f(400, 100));
    shapes.emplace_back(std::vector<sf::Vector2f>{{0, 0}, { 80, 0 }, { 40, 60 }}, sf::Vector2f(500, 300));
    shapes.emplace_back(std::vector<sf::Vector2f>{{0, 0}, { 20, 0 }, { 10, 15 }}, sf::Vector2f(250, 400));

    sf::CircleShape player;
    player.setPointCount(20);
    player.setFillColor(sf::Color(255, 0, 0));
    player.setRadius(3);

    float centerX = window.getSize().x / 2;
    float centerY = window.getSize().y / 2;
    sf::Vector2f centerOfWindow(centerX, centerY);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                player.setPosition(static_cast<float>(position.x) - player.getRadius(), static_cast<float>(position.y) - player.getRadius());
            }
        }

        window.clear(sf::Color::White);
        window.draw(player);
        Intersect intersect;
        for (const auto& shape : shapes) {
            window.draw(shape.shape);
        }
        for (const auto& shape : shapes) {
            for (int i = 0; i < shape.shape.getPointCount(); i++)
            {
                if (i == shape.shape.getPointCount() - 1)
                {
                    intersect = LineIntersect(player.getPosition(), centerOfWindow, shape.shape.getTransform().transformPoint(shape.shape.getPoint(0)),
                        shape.shape.getTransform().transformPoint(shape.shape.getPoint(i)));
                }
                else
                {
                    intersect = LineIntersect(player.getPosition(), centerOfWindow, shape.shape.getTransform().transformPoint(shape.shape.getPoint(i + 1)),
                        shape.shape.getTransform().transformPoint(shape.shape.getPoint(i)));
                }
                if (intersect.result == true)
                {
                    break;
                }
            }
            if (intersect.result == true)
            {
                break;
            }
        }
        if (intersect.result == true)
        {
            player.setPosition(intersect.pos);
        }
        drawLine(centerOfWindow, player.getPosition(), window, sf::Color::Red);
        window.display();
    }

    return 0;
}
