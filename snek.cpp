#include "graphics.hpp"
#include <vector>

using namespace genv;
using namespace std;
const int X = 900;
const int Y = 600;


struct position {
    float x, y, width, height;
};


struct apple {
private:
    float x, y, height, width;
    unsigned char r, g, b;

public:
    apple(float x, float y) {
        this->x = x;
        this->y = y;
        height = 50;
        width = 50;
        r = 255;
        g = 0;
        b = 0;
    }

    position getposition() {
        position positon{.x = x, .y = y, .width = width, .height = height};
        return positon;
    }


    void draw(){
        gout << move_to(x, y) << color(r, g, b) << box(width, height);
    }
};

struct snake {
private:
    float x, y, height, width, speed;
    unsigned char r, g, b;
public:
    snake(float x, float y) {
        this->x = x;
        this->y = y;
        height = 20;
        width = 20;
        speed = 1;
        r = 0;
        g = 255;
        b = 0;
    }

    bool collide(position pos) {
        if (x <= pos.x + pos.width && x >= pos.x){
            return true;
        }else if (y <= pos.y + pos.height && y <= pos.y){
            return true;
        }
        return false;
    };

    position getposition() {
        position positon{.x = x, .y = y, .width = width, .height = height};
        return positon;
    }

    void move() {
        gout << move_to(x, y) << color(r, g, b) << box(width, height);
    }
};

struct snakeHead : snake {
private:
    int direction = 0;
    vector<snake> body;
public:
    snakeHead(float x, float y) : snake(x,y) {
        body.emplace_back(x,y);
        for (int i = 0; i < 3; ++i) {
            body.emplace_back(x,y);
        }
    }

    void changedirection(int dir) {
        direction = dir;
    }

    void move_head(){
        position headpos = body.front().getposition();
        snake s{0,0};
        if (direction == 0){
            s = snake(headpos.x, headpos.y - headpos.height);
        }else if(direction == 1){
            s = snake(headpos.x + headpos.width, headpos.y);
        }else if( direction == 2 ){
            s = snake(headpos.x, headpos.y + headpos.height);
        }else if(direction == 3 ){
            s = snake(headpos.x - headpos.width, headpos.y );
        }
        body.insert(body.begin(), s);
        for(auto part : body){
            part.move();
        }
        body.pop_back();
    }

    void eat() {
        body.emplace_back(body.back());
    }

    bool check_collision(position pos){
        return body.front().collide(pos);
    }

    position getposition(){
        return body.front().getposition();
    }
};


struct wall {
private:
    float x, y, height, width;
    unsigned char r, g, b;
public:
    wall(float x, float y) {
        this->x = x;
        this->y = y;
        height = 50;
        width = 50;
        r = 255;
        g = 215;
        b = 0;
    }

    position getposition() {
        position positon{.x = x, .y = y, .width = width, .height = height};
        return positon;
    }

    void draw(){
        gout <<move_to(x, y) << color(r, g, b) << box(width, height);
    }
};

bool check_collision(apple apple, snakeHead snake) {
    return snake.check_collision(apple.getposition());
}

int main() {
    gout.open(X, Y);
    snakeHead player = snakeHead(X / 2, Y / 2);
    apple food(50, 50);
    vector<wall> walls;
    walls.reserve(60);
for (int i = 0; i < 18; ++i) {
        walls.emplace_back(i*50,0);
        walls.emplace_back(i*50,550);
    }
    for (int i = 0; i < 12; ++i) {
        walls.emplace_back(0,i*50);
        walls.emplace_back(850,i*50);
    }
    gout << refresh;
    gin.timer(100); //40ms = 25fps
    event ev;
    while (gin >> ev){
        gout << move_to(0, 0) << color(0, 0, 0) << box(X, Y) << color(255, 255, 255);
        gout << move_to(50, 50) << text("press enter to start or esc to exit") << color(0,0,0);
        if( ev.keycode == key_escape){
            return 0;
        }else if(ev.keycode == key_enter){
            break;
        }
        gout << refresh;
    }

    while (gin >> ev && ev.keycode != key_escape) {
        if (ev.keyname == "W" || ev.keyname == "Up") {
            player.changedirection(0);
        } else if (ev.keyname == "D" || ev.keyname == "Right") {
            player.changedirection(1);
        } else if (ev.keyname == "S" || ev.keyname == "Down") {
            player.changedirection(2);
        } else if (ev.keyname == "A" || ev.keyname == "Left") {
            player.changedirection(3);
        }

        if (ev.type == ev_timer) {
            gout << move_to(0, 0) << color(0, 0, 0) << box(X, Y) << color(255, 255, 255);
            player.move_head();
            food.draw();
            if(check_collision(food,player)){
                player.eat();
                food = apple(rand() % X + 50 , rand() % Y + 50);
            }else if(player.getposition().x < 50 ||
                    player.getposition().x +  player.getposition().width > X ||
                    player.getposition().y < 50 ||
                    player.getposition().y + player.getposition().height > Y){
                break;
            }
            for(wall wall : walls){
                wall.draw();
            }
            gout << refresh;
        }
    }

    while (gin >> ev){
        gout << move_to(0, 0) << color(0, 0, 0) << box(X, Y) << color(255, 255, 255);
        gout << move_to(X / 2, Y/2) << text("game over") << color(0,0,0);
        if( ev.keycode == key_escape){
            return 0;
        }
        gout << refresh;
    }
    return 0;
}
