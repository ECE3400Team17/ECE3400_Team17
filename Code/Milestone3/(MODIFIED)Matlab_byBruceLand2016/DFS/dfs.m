function brain = dfs(visited, robot_orient,maze,brain)


curr = visited(end);
robot_x_pos = mod(curr,100);
robot_y_pos = ((curr-robot_x_pos)/100);
sensorInfo = collectSensorInfo(robot_y_pos,robot_x_pos,robot_orient,maze);

neighbors = sensorInfo(1:3);
neighborsY = sensorInfo(4:6);
neighborsX = sensorInfo(7:9);
orients=[3,0,1];

while (length(neighbors)>0)
%     disp('Length Neighbors')
%     disp(length(neighbors))
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
%             disp('Case1!')
            visited = [visited (nextY*100+nextX)];
            brain(nextY,nextX) = (nextY*100+nextX);
            
            robot_orient = mod(robot_orient+nextO,4);
            move(robot_orient,robot_y_pos,robot_x_pos,maze);
            
%             disp(brain)
            brain = dfs(visited,robot_orient,maze,brain);
            return
        else
%             disp('Case2!')
        end
    else
%         disp('Case3!')
        brain(nextY,nextX) = 1;
    end
    
end

if length(neighbors)==0
    if length(visited)==1
%         disp(brain)
        return
    end
%     disp('Case4!')
    
    old = visited(end-1);
    robot_x_posOLD = mod(old,100);
    robot_y_posOLD = ((old-robot_x_posOLD)/100);
    
    diffX = robot_x_posOLD-robot_x_pos;
    diffY = robot_y_posOLD-robot_y_pos;
    
    nextO = backTrack(robot_orient,diffY,diffX);

    
    robot_orient = mod(robot_orient+nextO,4);
    move(robot_orient,robot_y_pos,robot_x_pos,maze);

    brain = dfs(visited(1:end-1),robot_orient,maze,brain);
    return
    
end



end
