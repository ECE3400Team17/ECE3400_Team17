function [rY,rX,lY,lX,bY,bX]= collectSensorInfoCorr(robot_orient)

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
        lR = robot_x_pos;
        
        front_wall_sensor = maze(robot_y_pos, robot_x_pos+1);
        fY = robot_y_pos;
        fR = robot_x_pos+1;
        
        bY = robot_y_pos;
        bX = robot_x_pos-1;
        
    elseif (robot_orient==2)
        right_wall_sensor = maze(robot_y_pos, robot_x_pos-1);
        rY = robot_y_pos ;
        rX = robot_x_pos-1;
        
        left_wall_sensor = maze(robot_y_pos, robot_x_pos+1);
        lY = robot_y_pos;
        lR = robot_x_pos+1;
        
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
    
end