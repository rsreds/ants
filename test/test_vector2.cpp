//
// Created by Jacopo Gasparetto on 26/10/21.
//


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "test/doctest.h"
#include <vector2/vector2.h>

constexpr float epsilon = 1e-4;

TEST_CASE("Magnitude") {
    std::cout << std::string(5, '*') << " Magnitude " << std::string(5, '*') << '\n';
    sf::Vector2f v0 {4, 9};
    float m = sf::getMagnitude(v0);
    float trueMagnitude = 9.8489f;
    std::cout << "Magnitude " << "v{" << v0 << "}: " << m << '\n';
    CHECK_LE(abs(m - trueMagnitude), epsilon);

    sf::Vector2f p1{ 4, 3 };
    sf::Vector2f p2{ 3, -2 };
    float distance{ sf::distanceBetween(p1, p2) };
    float trueDistance = 5.099f;
    std::cout << "Distance between: {" << p1 << "} and {" << p2 << "}: " << distance << '\n';
    CHECK_LE(abs(distance - trueDistance), epsilon);
}

TEST_CASE("Normalization") {
    std::cout << std::string(5, '*') << " Normalization " << std::string(5, '*') << '\n';
    sf::Vector2f v0{ 13, 8 };
    sf::Vector2f v0_copy{v0};
    auto v_temp{ sf::normalize(v0) };

    std::cout << "Normalized vector v0{" << v0_copy <<"}: " << v0 << '\n';
    CHECK_LE(abs(v0.x - 0.8517), epsilon);
    CHECK_LE(abs(v0.y - 0.5241), epsilon);

    std::cout << "Normalized vector ref: " << v_temp << '\n';
    CHECK_LE(abs(v_temp.x - 0.8517), epsilon);
    CHECK_LE(abs(v_temp.y - 0.5241), epsilon);
    
    sf::Vector2f v1{ 13, 8 };
    auto v_temp1{ sf::normalizeCopy(v1) };

    CHECK_LE(abs(v1.x - 13), epsilon);
    CHECK_LE(abs(v1.y - 8), epsilon);

    std::cout << "Normalized copy v1{" << v1 <<"}: {" << v_temp1 << "}\n";
    CHECK_LE(abs(v_temp1.x - 0.8517), epsilon);
    CHECK_LE(abs(v_temp1.y - 0.5241), epsilon);
    
    v0 = sf::Vector2f {17, 12};
    v1 = v0;

    std::cout << "Magnitude {" << v1 << "} before: " << sf::getMagnitude(v1);
    sf::limitMagnitude(v1, 5.f);

    // Check vector basis is invariant after magnitude cap
    CHECK_EQ(sf::normalizeCopy(v1), sf::normalizeCopy(v0));
    std::cout << ", magnitude {" << v1 << "} after: " << getMagnitude(v1) << '\n';
    CHECK_LE(getMagnitude(v1) - 5.f, epsilon);
}

TEST_CASE("Trigonometry") {
    std::cout << std::string(5, '*') << " Trigonometry " << std::string(5, '*') << '\n';

    sf::Vector2f u { 2, 2};
    sf::Vector2f v { -4, -4};
    float u_dot_v = sf::dotProduct(u, v);

    std::cout << "Dot product: {" << u << "} • {" << v << "} = " << u_dot_v << '\n';
    CHECK_LE(abs(abs(u_dot_v) - sf::getMagnitude(u) * sf::getMagnitude(v)), epsilon);

    u = {5, 6};
    v = {-6, 5};
    u_dot_v = sf::dotProduct(u, v);
    std::cout << "Dot product: {" << u << "} • {" << v << "} = " << u_dot_v << '\n';
    CHECK_LE(abs(u_dot_v), epsilon);

    u = { 8, 0 };
    v ={ 0, 13 };

    auto angle = sf::angleBetween(u, v);
    std::cout << "Angle between u{" << u << "}, v{" << v << "} is: " << angle << '\n';
    CHECK_LE(abs(angle - sf::PI / 2), epsilon);
    auto deg = sf::rad2deg(angle);
    std::cout << "Angle "<< angle << " = " << deg << "°\n";
    CHECK_LE(abs(deg - 90), epsilon);

    u = {3, 4};
    v = {-2, 5};
    angle = sf::angleBetween(u, v);
    std::cout << "Angle between u{" << u << "}, v{" << v << "} is: " << angle << '\n';
    CHECK_LE(abs(angle - 1.02401f), epsilon);

    deg = sf::rad2deg(angle);
    std::cout << "Angle "<< angle << " = " << deg << "°\n";
    CHECK_LE(abs(deg - 58.6713), epsilon);
}