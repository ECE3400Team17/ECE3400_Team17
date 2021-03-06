% Bruce Land
% robot simulator
clear all;
figure(1); clf;
clc;

dt = 0.1; % sec
time_end = 100 ;
time = 0:dt:time_end;

%maze structure maze(x,y)   
% walls on odd indexs
wall = 1; % zero means no wall
no_wall = 0;
robot = 2;
% start with no walls
maze = zeros(11,11) ;
% wall around the whole maze
maze(:,1) = wall;
maze(:,11) = wall;
maze(1,:) = wall;
maze(11,:) = wall;
% interior walls
maze(1:5,3) = wall;
maze(7:11,3) = wall;
maze(5,3:7) = wall ;
maze(7,3:5) = wall ;
maze(7,6:7) = wall ; %%
maze(9,3:9) = wall ;
maze(9,3:9) = wall ;
maze(4:8,9) = wall ;
maze(1:3,5) = wall ;
maze(1:3,7) = wall ;
% imagesc(maze)
% axis xy
% return
% init state to first intersection 
% inside the lower left corner
robot_x_pos = 2;
robot_y_pos = 2;
maze(robot_y_pos,robot_x_pos) = robot;
%==========================================
% orientation  0--facing +y direction
%            3   1--facing +x direction
%              2
%==========================================
robot_orient = 0;
% first step is always up
x_inc = 0; y_inc = 1;

imagesc(maze)
robot_eyes = text(1.9,2.3,'O');
colormap spring
xlabel 'x'
ylabel 'y'
axis tight
axis xy

%assume distance sensors
brain = ones(size(maze,1),size(maze,2));

% for t=time
   visited = robot_y_pos*100+robot_x_pos;
   brain(robot_y_pos,robot_x_pos) = visited;
   
   brain = dfs(visited , robot_orient,maze,brain);
   disp(flipud(brain))
   
   disp('Finished!')
  
  
% end %time



