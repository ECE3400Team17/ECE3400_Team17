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