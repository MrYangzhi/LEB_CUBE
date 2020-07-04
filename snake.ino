#include <Adafruit_NeoPixel.h>
#include <stdlib.h>

#define PIN        4 
#define NUMPIXELS 64 



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

char led_matrix[8][8] = {
  {0, 15, 16, 31, 32, 47, 48, 63},
  {1, 14, 17, 30, 33, 46, 49, 62},
  {2, 13, 18, 29, 34, 45, 50, 61},
  {3, 12, 19, 28, 35, 44, 51, 60},
  {4, 11, 20, 27, 36, 43, 52, 59},
  {5, 10, 21, 26, 37, 42, 53, 58},
  {6, 9,  22, 25, 38, 41, 54, 57},
  {7, 8,  23, 24, 39, 40, 55, 56}
};

typedef struct{
  int x;
  int y;
}coord_t;

enum snake_head_t{
  up,
  down,
  left,
  right
};

//食物的位置
int randNumber;    //随机数
coord_t food;       //转化为x y坐标
coord_t randNumber_to_coord_t(int randNumber)
{
  coord_t coord;
  coord.x = randNumber / 8;
  coord.y = randNumber % 8;
  return coord;
}

class Snake 
{
  public:
    int snakeLen = 3;       //蛇的长度，最大脚标为头
    //蛇身体的位置蛇
    int *snakeLenX = new int[snakeLen];
    int *snakeLenY = new int[snakeLen];

    //是否迟到食物
    bool eat_food;
  
    //蛇头朝向
    snake_head_t snake_head;
    Snake()             //蛇位置初始化
    {
      snakeLenX[0] = 0;
      snakeLenX[1] = 0;
      snakeLenX[2] = 0;
      
      snakeLenY[0] = 0;
      snakeLenY[1] = 1;
      snakeLenY[2] = 2;

      snake_head = right;

      eat_food = true;          //开局产生下一个食物
    }

    bool get_eat_food(void){
      return eat_food;
    }
    
    void move_next(coord_t food)     //向下一个食物移动 先处理行，再处理列
    { 
      //x为行
      if(snakeLenX[4] < food.x) {                 //食物在蛇头的下面列
        if(snake_head != up)        //只要不朝上就朝下
          snake_head = down;
        else{                       //朝上的时候根据列决定朝向
          if(snakeLenY[4] >= food.y)
            snake_head = left;
          else 
            snake_head = right;            
        }
          
      }else if( snakeLenX[4] = food.x ){          //在同一行，考虑列
        if(snakeLenY[4] < food.y){                
          snake_head = right;
        }else if( snakeLenY[4] = food.y)          //吃到了食物，产生下一个食物
        {
          eat_food = true;
        }else if(snakeLenY[4] > food.y){
          snake_head = left;
        }
      }else if(snakeLenX[4] > food.x) {           //食物在蛇头的上面行,向上面行
        if(snake_head != down)
          snake_head = up;
        else{
          if(snakeLenY[4] < food.y){                
            snake_head = right;
          }else if(snakeLenY[4] >= food.y){
            snake_head = left;
          }
        }
      }

    }
};
Snake snake;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  randomSeed(500);
  Serial.begin(115200);
  Serial.println("snake game");
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'

  //打印蛇的位置
  for(int i=0; i<snake.snakeLen; i++) { // For each pixel...
    pixels.setPixelColor(led_matrix[snake.snakeLenX[i]][snake.snakeLenY[i]], pixels.Color(0, 1, 0));
  }
  if( snake.get_eat_food() )      //吃到了食物产生下一个食物
  {
    snake.eat_food = false;
    while(true)                   //产生一个食物，不与蛇的身体重合
    {
      randNumber = random(64);      //0~63
      food = randNumber_to_coord_t(randNumber);
      if(food.x == snake.snakeLenX[0] && food.y == snake.snakeLenY[0] ){   //与蛇身体重合则重来
        continue;
      }else if( food.x == snake.snakeLenX[1] && food.y == snake.snakeLenY[1] ){
        continue;
      }else if( food.x == snake.snakeLenX[2] && food.y == snake.snakeLenY[2] ){
        continue;
      }else
      {
        break;
      }
    }
    Serial.print("new food is in x:");
    Serial.print(food.x);
    Serial.print("  y:");
    Serial.println(food.y);
  }
  pixels.setPixelColor(led_matrix[food.x][food.y], pixels.Color(5, 0, 0));
  //显示
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(DELAYVAL); // Pause before next pass through loop
}
