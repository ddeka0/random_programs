clear all;
clc;
fid = fopen('result.txt');
result = zeros(101,101);
no_of_rows = 101;
no_of_col = 101;
pause(5);
for step = 1:3
    %fprintf("%d\n",step );
    for i=1:101
        for j=1:101
            result(i,j) = fscanf(fid,'%f',1);
        end
    end
    %colormap hsv
	surf(result,'FaceColor','interp',...
   		'EdgeColor','none',...
   		'FaceLighting','gouraud')
	%daspect([5 5 1])
	%axis tight
    pause(2);
end
%surf(result);
	
	