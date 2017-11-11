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