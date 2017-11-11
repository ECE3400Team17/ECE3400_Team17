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
imagesc(maze)
% axis xy
% return

%%

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
%%
robot_eyes = text(1.9,2.3,'O');
colormap spring
xlabel 'x'
ylabel 'y'
axis tight
axis xy

%%
%assume distance sensors

brain = zeros(size(maze,1),size(maze,2),2);
priceF = 1;
priceR = 2;
priceL = 2;

brain(robot_y_pos,robot_x_pos,1)=3;
disp(brain)
for t=time

    %update sensors -- body centered coods
    brain(robot_y_pos,robot_x_pos,2) = 2; % set current location as visited
    spent = brain(robot_y_pos,robot_x_pos,1);%the cost we paid to get to currrent location
    
    % collect sensor information
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
    
    cb = brain(bY,bX,1);
    vb = brain(bY,bX,2);
    cr = 0;
    cl = 0;
    cf = 0;
    vr = 1;
    vl = 1;
    vf = 1;
    
    if(right_wall_sensor==no_wall)
        rC = brain(rY,rX,1);
        if rC == 0
            cr = spent+priceR;
        else
            cr = rC;
        end
        brain(rY,rX,1)=cr;% calculate the price of turning right
        vr = brain(rY,rX,2);% check if right was visited
    else % if wall
        cr = 1e7;
        vr = 1;
        brain(rY,rX,1)=cr; % remember wall
        brain(rY,rX,2)=vr; % remember wall
    end
    
    if(left_wall_sensor==no_wall)
        lC = brain(lY,lX,1);
        if lC == 0
            cl = spent+priceL;
        else
            cl = lC;
        end
        
        brain(lY,lX,1)=cl;% calculate the price of turning left
        vl = brain(lY,lX,2);% check if left was visited
    else % if wall
        cl = 1e7;
        vl = 1;
        brain(lY,lX,1)=cl; % remember wall
        brain(lY,lX,2)=vl; % remember wall
    end
    
    if(front_wall_sensor==no_wall)
        fC = brain(fY,fX,1);
        if fC == 0
            cf = spent+priceF;
        else
            cf = fC;
        end
        
        brain(fY,fX,1)=cf;% calculate the price of going front
        vf = brain(fY,fX,2);% check if front was visited
    else % if wall
        cf = 1e7;
        vf = 1;
        brain(fY,fX,1)=cf; % remember wall
        brain(fY,fX,2)=vf; % remember wall
    end
    
    nextY = 0;
    nextX = 0;
    nextOr = 0;
    
    %     yMoves = [lY,fY,rY];
    %     xMoves = [lX,fX,rX];
    
    cost = [cl,cf,cr,cb];
    visted = [vl,vf,vr,vb];
    [sortC,sortI] = sort(cost);
    disp(flipud(brain))
    disp(cost)
    disp(visted)
    
    % Decision
    if (visted(1)==1) && (visted(2)==1) && (visted(3)==1) % deadend
        nextOr = 2; % go back
        disp('here1')
    elseif (visted(sortI(1))==0 && (sortI(1))~= 4)
        nextOr = sortI(1)-2;
        disp('here2')
    elseif (visted(sortI(2))==0)
        nextOr = sortI(2)-2;
        disp('here3')
    elseif (visted(sortI(3))==0)
        nextOr = sortI(3)-2;
        disp('here4')
    elseif (visted(sortI(4))==0)
        nextOr = sortI(4)-2;
        disp('here5')
    elseif (visted(sortI(1))~=1)
        nextOr = sortI(1)-2;
        disp('here6')
    elseif (visted(sortI(2))~=1)
        nextOr = sortI(2)-2;
        disp('here7')
    elseif (visted(sortI(3))~=1)
        nextOr = sortI(3)-2;
        disp('here8')
    elseif (visted(sortI(4))~=1)
        nextOr = sortI(4)-2;
        disp('here9')
    else
        nextOr = sortI(1)-2;
        disp('here10')
        disp('DONE!')
    end
    


    robot_orient = mod(robot_orient+nextOr,4);
    
    disp(robot_orient);
    
    % erase old robot location
    maze(robot_y_pos,robot_x_pos) = no_wall;
    
    turning = 0;

    
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
    
    
        robot_y_pos = robot_y_pos + y_inc;
        robot_x_pos = robot_x_pos + x_inc;
        maze(robot_y_pos,robot_x_pos) = robot;
    
    pause()
    % draw it
    imagesc((maze))
    robot_eyes = text(robot_x_pos+eye_x,robot_y_pos+eye_y,'O');
    colormap spring
    xlabel 'x'
    ylabel 'y'
    axis tight
    axis xy
    drawnow
    
    
end %time



