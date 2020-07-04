#include <Adafruit_NeoPixel.h>
#include <stdlib.h>

#define PIN         4 
#define ROW         8           //行
#define COLUMN      32          //列
#define NUMPIXELS   (64*5)
#define SNAKE_FOOD  (64*4)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels


/*
 * 
 * 8*8矩阵  根据灯面阵的特点来的
 * 
 * x：行数       y：列数
 * 
  0 15 16 31 32 47 48 63
  1 14 17 30 33 46 49 62
  2 13 18 29 34 45 50 61
  3 12 19 28 35 44 51 60
  4 11 20 27 36 43 52 59
  5 10 21 26 37 42 53 58
  6 9  22 25 38 41 54 57
  7 8  23 24 39 40 55 56
  */

int led_matrix[ROW][COLUMN] = {
  {64,79,80,95,96 ,111,112,127,128,143,144,159,160,175,176,191,192,207,208,223,224,239,240,255,256,271,272,287,288,303,304,319},                //64 + (-1)^x 
  {65,78,81,94,97 ,110,113,126,129,142,145,158,161,174,177,190,193,206,209,222,225,238,241,254,257,270,273,286,289,302,305,318},                //
  {66,77,82,93,98 ,109,114,125,130,141,146,157,162,173,178,189,194,205,210,221,226,237,242,253,258,269,274,285,290,301,306,317},                //
  {67,76,83,92,99 ,108,115,124,131,140,147,156,163,172,179,188,195,204,211,220,227,236,243,252,259,268,275,284,291,300,307,316},                //
  {68,75,84,91,100,107,116,123,132,139,148,155,164,171,180,187,196,203,212,219,228,235,244,251,260,267,276,283,292,299,308,315},
  {69,74,85,90,101,106,117,122,133,138,149,154,165,170,181,186,197,202,213,218,229,234,245,250,261,266,277,282,293,298,309,314},
  {70,73,86,89,102,105,118,121,134,137,150,153,166,169,182,185,198,201,214,217,230,233,246,249,262,265,278,281,294,297,310,313},
  {71,72,87,88,103,104,119,120,135,136,151,152,167,168,183,184,199,200,215,216,231,232,247,248,263,264,279,280,295,296,311,312}
};

typedef struct{
  int x;
  int y;
}coord_t;

enum snake_head_t{
  none,
  up,
  down,
  left,
  right
};

class Snake{
public:
  static const int body_num = 10;
  coord_t body[body_num];    

  bool eat_food;
  snake_head_t snake_head;

  Snake()
  {
    for(int i=0; i < body_num; i++){
      body[i].x = 0;
      body[i].y = body_num-1;
    }
    eat_food = false;
    snake_head = none;
  }
  void snake_move(coord_t food){

    //蛇头
    if(body[0].x > food.x){             //行移动
      body[0].x--;
    }else if(body[0].x == food.x){      //列移动
      if(body[0].y > food.y){           //列左移
        body[0].y--;
      }
      else if(body[0].y < food.y){      //列右移
          body[0].y++;
      }
      else{                           //吃到了食物
        eat_food = true;
      }
    }else if(body[0].x < food.x){
      body[0].x++;
    }
    //身体
    for(int i = body_num -1; i > 0; i--){
      body[i] = body[i-1];
    }
  }
};

Snake snake;

//食物的位置
coord_t food;               //食物的x y坐标


//将随机数转为坐标
coord_t randNumber_to_coord_t(int randNumber)
{
  coord_t coord;
  coord.x = randNumber / COLUMN;
  coord.y = randNumber % COLUMN;
  return coord;
}

//产生下一个食物，避开蛇的身体
coord_t next_food(coord_t snake)      
{
  coord_t food;
  while(true)
  {
    int randNumber = random(SNAKE_FOOD);      //
    food = randNumber_to_coord_t(randNumber);
    if( (food.x != snake.x) && (food.y != snake.y) )
      break;
  }
  return food;
}

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  randomSeed(200);
  Serial.begin(115200);
  Serial.println("snake game");
  next_food(snake.body[0]);
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  if(snake.eat_food){
    snake.eat_food = false;
    food = next_food(snake.body[0]);
    
  }
  pixels.setPixelColor(led_matrix[food.x][food.y], pixels.Color(50, 0, 0));
  snake.snake_move(food);

  for(int i = 0; i < snake.body_num; i++){
    pixels.setPixelColor(led_matrix[snake.body[i].x][snake.body[i].y], pixels.Color(0, 30- 3 * i, 0));
  }
   
  //显示
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(DELAYVAL-random(DELAYVAL/2)); // Pause before next pass through loop
}
