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