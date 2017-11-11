[To Home Page](./index.md)

# Milestone3

## Objective:
The first goal of this milestone is to use the simulation to write a maze solving algorithum. The second goal of this milestone was to start putting all the past labs and milestones together an implement our search algorithum in real-life. 

## In Simulation:

We are using the DFS (Depth First Search) method to solve the maze. For any maze the robot is searching through, we could treat the maze as a tree structure with the top node as the starting point and one leaf node as the exit (ending point). Then, by applying Depth-First Searching recursion algorithm, we could lead the robot searching through the maze.

For example, with the maze given below:

![](./images/Milestone3/pic1.png)

We could represent the maze as the tree given below:

![](./images/Milestone3/pic2.png)


( ** represent the finishing the maze)

While depth-first searching the maze (in-order traverse with backtracking), we would get the robot moving path as below:

A B D (backtrack) E H L (backtrack) M ** 




### Overview of DFS Algorithum:
We tried out our algorithum in matlab and use the simulation given by Professor Bruce Land.  We split the simulation to funtions like:
* collectSensorInformation.m = Outputs the information from the three wall sensors (left, front, and right wall sensors)
* dfs.m = traces the maze recursively. 

At each grid point the robot collects information about all it's neighbors which will help it decide whether any of the neigbors are a wall or a path to trace. If it reaches a deadend, the robot will trace back to the last time it reached an intersection with multiple options to go down (a fork in the rode).  It prints out Done! when it has mapped out the entire maze.

The matlab code is provided at the bottom.

The vedio below shows that the simulation: 

 <iframe width="560" height="315" src="https://www.youtube.com/embed/Ljgl3WTfQXA" frameborder="0" gesture="media" allowfullscreen></iframe>
 
 ### Overview of Optimized Algorithum:
The DFS is our stepping stone.  We started optimizing our algorithum by weighing the cost at each intersection and choosing the path with the lower cost.  At a deadend, the robot will follow the shortest path back to the most recent intersection with other potential paths.
 
 

## In Real Life:

### Line & Wall sensors:

We’re using 4 line sensors (product #) to let the robot go straight along the black tape and detecting the cross section. The most implementation of this part will be the same as what we did in milestone 1 and 2. For the extra requirement of milestone 3, we will treat each cross section we detected as a decision-making place where the robot will collect the information from the three wall detectors and to decide which way it will keep going based on the DFS algorithm.

The wall detectors we’re using are VL53L0X () sensors, which could detect the distance between the sensor and objects in front at a very accurate level. The sensors need I2C interface for data transmission between itself and the Arduino. Also for multiple usage for the sensor (we are using three of them in total), we cannot activate them simultaneously and have to alternate between the sensors. 

To alternate between sensors to get all wall information, we will assign each sensor a unique address via a XSHUT pin. The Arduino code for alternating multiple wall sensors is below (we were alternating between two wall sensors in the test code):

```arduino

#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int counter;

void setup() {
  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  /*
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  */
  // power
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  delay(10);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(9,LOW); 
  lox.begin(0x30);
  digitalWrite(9,HIGH);
  lox.begin(0x31);
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));

}


void loop() {
  digitalWrite(9,LOW);
  digitalWrite(8,HIGH);
  counter = 0;
  while(counter <= 10){
    detect1();
    counter = counter + 1;
  }
  counter = 0;
  digitalWrite(9,HIGH);
  digitalWrite(8,LOW);
  //lox.begin(0x31);
  
  while(counter <= 10){
    detect2();
    counter = counter + 1;
  }
  
}

void detect1() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance(1) (mm): "); Serial.println(measure.RangeMilliMeter);
  } 
  else {
    Serial.println(" out of range ");
  }
   
  delay(100);

}

void detect2() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance(2) (mm): "); Serial.println(measure.RangeMilliMeter);
  } 
  else {
    Serial.println(" out of range ");
  }
   
  delay(100);

} 
```

## To do list:
* Work out the motor and IMU
* Integrate Radio & SPI transmission (data for maze drawing by figuring out the packet)
* Draw the Maze based on the data transmitted

## Matlab Simulation Code:

### Collecting Wall Sensor Data Information:
```matlab
function out= collectSensorInfo(robot_y_pos, robot_x_pos,robot_orient,maze)

% facing north
if (robot_orient==0)
    right_wall_sensor = maze(robot_y_pos, robot_x_pos+1);
    rY = robot_y_pos;
    rX = robot_x_pos+1;
    
    left_wall_sensor = maze(robot_y_pos, robot_x_pos-1);
    lY = robot_y_pos;
    lX = robot_x_pos-1;
    
    front_wall_sensor = maze(robot_y_pos+1, robot_x_pos);
    fY = robot_y_pos+1;
    fX = robot_x_pos;
    
    bY = robot_y_pos-1;
    bX = robot_x_pos;
    
% facing east    
elseif (robot_orient==1)
    right_wall_sensor = maze(robot_y_pos-1, robot_x_pos);
    rY = robot_y_pos-1;
    rX =  robot_x_pos ;
    
    left_wall_sensor = maze(robot_y_pos+1, robot_x_pos);
    lY =robot_y_pos+1 ;
    lX = robot_x_pos;
    
    front_wall_sensor = maze(robot_y_pos, robot_x_pos+1);
    fY = robot_y_pos;
    fX = robot_x_pos+1;
    
    bY = robot_y_pos;
    bX = robot_x_pos-1;
    
% facing south
elseif (robot_orient==2)
    right_wall_sensor = maze(robot_y_pos, robot_x_pos-1);
    rY = robot_y_pos ;
    rX = robot_x_pos-1;
    
    left_wall_sensor = maze(robot_y_pos, robot_x_pos+1);
    lY = robot_y_pos;
    lX = robot_x_pos+1;
    
    front_wall_sensor = maze(robot_y_pos-1,robot_x_pos);
    fY = robot_y_pos-1;
    fX = robot_x_pos;
    
    bY = robot_y_pos+1;
    bX = robot_x_pos;
    
% facing west
elseif (robot_orient==3)
    right_wall_sensor = maze(robot_y_pos+1, robot_x_pos);
    rY = robot_y_pos+1;
    rX = robot_x_pos;
    
    left_wall_sensor = maze(robot_y_pos-1, robot_x_pos);
    lY = robot_y_pos-1;
    lX = robot_x_pos;
    
    front_wall_sensor = maze(robot_y_pos, robot_x_pos-1);
    fY = robot_y_pos;
    fX = robot_x_pos-1;
    
    bY = robot_y_pos;
    bX = robot_x_pos+1;
    
end

% output sensor information
out = [left_wall_sensor,front_wall_sensor,right_wall_sensor,lY,fY,rY,lX,fX,rX];
end
```

### Move the location of the Robot:
```matlab
function move(robot_orient,robot_y_pos,robot_x_pos,maze)

% motion settings
if (robot_orient==0)
    x_inc = 0; y_inc = 1;
    eye_x = -.1 ; eye_y=0.3;
elseif (robot_orient==1)
    x_inc = 1; y_inc = 0;
    eye_x = .3 ; eye_y=0.1;
elseif (robot_orient==2)
    x_inc = 0; y_inc = -1;
    eye_x = 0 ; eye_y=-0.3;
elseif (robot_orient==3)
    x_inc = -1; y_inc = 0;
    eye_x = -0.5 ; eye_y=-0.1;
end

maze(robot_y_pos,robot_x_pos) = 0;
robot_y_pos = robot_y_pos + y_inc;
robot_x_pos = robot_x_pos + x_inc;
maze(robot_y_pos,robot_x_pos) = 2;

pause(0.1)
% draw it
imagesc(maze)
robot_eyes = text(robot_x_pos+eye_x,robot_y_pos+eye_y,'O');
colormap spring
xlabel 'x'
ylabel 'y'
axis tight
axis xy
drawnow


end
```

### Back Track:
```matlab
function nextO = backTrack(robot_orient,diffY,diffX)

    if robot_orient == 0
        if diffY==0
            if diffX==1
                nextO = 1;
            else
                nextO = 3;
            end
        else
            if diffY==1
                nextO = 0;
            else
                nextO = 2;
            end
        end
        
        
    elseif robot_orient == 1
        if diffY==0
            if diffX==1
                nextO = 0;
            else
                nextO = 2;
            end
        else
            if diffY==1
                nextO = 3;
            else
                nextO = 1;
            end
        end
        
    elseif robot_orient == 2
        if diffY==0
            if diffX==1
                nextO = 3;
            else
                nextO = 1;
            end
        else
            if diffY==1
                nextO = 2;
            else
                nextO = 0;
            end
        end
        
    else
        if diffY==0
            if diffX==1
                nextO = 2;
            else
                nextO = 0;
            end
        else
            if diffY==1
                nextO = 1;
            else
                nextO = 3;
            end
        end
    end

end
```


### Recursive DFS:
```matlab
function brain = dfs(visited, robot_orient,maze,brain)

if (isempty(find(brain==0))) % when map is finished
    disp('Done!')
    return
end

curr = visited(end);
robot_x_pos = mod(curr,100);
robot_y_pos = ((curr-robot_x_pos)/100);
sensorInfo = collectSensorInfo(robot_y_pos,robot_x_pos,robot_orient,maze);

neighbors = sensorInfo(1:3);
neighborsY = sensorInfo(4:6);
neighborsX = sensorInfo(7:9);
orients=[3,0,1];

while (length(neighbors)>0)
    
    % get all neighbors
    nextN = neighbors(1);
    nextY = neighborsY(1);
    nextX = neighborsX(1);
    nextO = orients(1);
    
    neighbors(1) = [];
    neighborsY(1) = [];
    neighborsX(1) = [];
    orients(1) = [];
    
    if nextN ~= 1 % not a wall
        if (isempty(find(brain==(nextY*100+nextX))))
            % mark position as visited
            visited = [visited (nextY*100+nextX)]; 
            brain(nextY,nextX) = 2;
            
            robot_orient = mod(robot_orient+nextO,4);
            move(robot_orient,robot_y_pos,robot_x_pos,maze);
            
            % do DFS again
            brain = dfs(visited,robot_orient,maze,brain);
            return
        end
    else
        % mark position as wall
        brain(nextY,nextX) = 1;
    end
    
end

% if we hit a deadend
if length(neighbors)==0
    if length(visited)==1
        return
    end
    
    old = visited(end-1);
    robot_x_posOLD = mod(old,100);
    robot_y_posOLD = ((old-robot_x_posOLD)/100);
    
    diffX = robot_x_posOLD-robot_x_pos;
    diffY = robot_y_posOLD-robot_y_pos;
    
    % Go back to most recent node
    nextO = backTrack(robot_orient,diffY,diffX);
    
    robot_orient = mod(robot_orient+nextO,4);
    move(robot_orient,robot_y_pos,robot_x_pos,maze);
    
    % do DFS again
    brain = dfs(visited(1:end-1),robot_orient,maze,brain);
    return
    
end

end

```


[To Home Page](./index.md)
