function avg_time = read_file(file)

    fileID = fopen(file,'r');
    sum = 0;
    for i = 1:10
       fscanf(fileID, '%f %f');
       fscanf(fileID, '%f %f');
       sum = sum + fscanf(fileID, 'Time elapsed with MPI clock = %f');
    end
    fclose(fileID);
    avg_time = sum/10;
end