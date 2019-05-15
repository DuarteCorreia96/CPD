test.node = 1;
test.core = 1;
test.time = read_file('mpi_1_1.out');

tests = test;

test.node = 1;
test.core = 2;
test.time = read_file('mpi_1_2.out');

tests = [tests test];

test.node = 1;
test.core = 4;
test.time = read_file('mpi_1_4.out');

tests = [tests test];

test.node = 5;
test.core = 1;
test.time = read_file('mpi_5_1.out');

tests = [tests test];

test.node = 5;
test.core = 2;
test.time = read_file('mpi_5_2.out');

tests = [tests test];

test.node = 5;
test.core = 4;
test.time = read_file('mpi_5_4.out');

tests = [tests test];

test.node = 10;
test.core = 1;
test.time = read_file('mpi_10_1.out');

tests = [tests test];

test.node = 10;
test.core = 2;
test.time = read_file('mpi_10_2.out');

tests = [tests test];

test.node = 10;
test.core = 4;
test.time = read_file('mpi_10_4.out');

tests = [tests test];

test.node = 20;
test.core = 1;
test.time = read_file('mpi_20_1.out');

tests = [tests test];

test.node = 20;
test.core = 2;
test.time = read_file('mpi_20_2.out');

tests = [tests test];

test.node = 20;
test.core = 4;
test.time = read_file('mpi_20.out');

tests = [tests test];

test.node = 30;
test.core = 1;
test.time = read_file('mpi_30_1.out');

tests = [tests test];

test.node = 30;
test.core = 2;
test.time = read_file('mpi_30_2.out');

tests = [tests test];

test.node = 30;
test.core = 4;
test.time = read_file('mpi_30.out');

tests = [tests test];

test.node = 40;
test.core = 1;
test.time = read_file('mpi_40_1.out');

tests = [tests test];

test.node = 40;
test.core = 2;
test.time = read_file('mpi_40_2.out');

tests = [tests test];

test.node = 40;
test.core = 4;
test.time = read_file('mpi_40.out');

tests = [tests test];

test.node = 50;
test.core = 1;
test.time = read_file('mpi_50_1.out');

tests = [tests test];

test.node = 50;
test.core = 2;
test.time = read_file('mpi_50_2.out');

tests = [tests test];

test.node = 50;
test.core = 4;
test.time = read_file('mpi_50.out');

tests = [tests test];

test.node = 60;
test.core = 1;
test.time = read_file('mpi_60_1.out');

tests = [tests test];

test.node = 60;
test.core = 2;
test.time = read_file('mpi_60_2.out');

tests = [tests test];

test.node = 60;
test.core = 4;
test.time = read_file('mpi_60.out');

tests = [tests test];

test.node = 70;
test.core = 1;
test.time = read_file('mpi_70_1.out');

tests = [tests test];

test.node = 70;
test.core = 2;
test.time = read_file('mpi_70_2.out');

tests = [tests test];

test.node = 70;
test.core = 4;
test.time = read_file('mpi_70.out');

tests = [tests test];

test.node = 80;
test.core = 1;
test.time = read_file('mpi_80_1.out');

tests = [tests test];

test.node = 80;
test.core = 2;
test.time = read_file('mpi_80_2.out');

tests = [tests test];

test.node = 80;
test.core = 4;
test.time = read_file('mpi_80.out');

tests = [tests test];

test1_node = [];
test1_time = [];

test2_node = [];
test2_time = [];

test4_node = [];
test4_time = [];

for i = 1:length(tests)
    if 1 == tests(i).core
        test1_node(end+1) = tests(i).node;
        test1_time(end+1) = tests(i).time;
    end
    if 2 == tests(i).core
        test2_node(end+1) = tests(i).node;
        test2_time(end+1) = tests(i).time;
    end
    if 4 == tests(i).core
        test4_node(end+1) = tests(i).node;
        test4_time(end+1) = tests(i).time;
    end
end

close all
figure(1)
ax1 = subplot(2,2,1);
plot(test1_node, test1_time)
hold on

plot(test2_node, test2_time)
hold on

plot(test4_node, test4_time)
hold on

grid minor
title('Execution Time ./simpar-mpi 2 20 100000000 5','Interpreter', 'Latex');
ylabel('Execution Time [seconds]', 'Interpreter', 'Latex');
xlabel('Number of nodes used','Interpreter', 'Latex');
legend('1 core', '2 cores', '4 cores');

figure(1)
ax2 = subplot(2,2,2);
plot(test1_node, test1_time(1)./test1_time)
hold on

plot(test2_node, test1_time(1)./test2_time)
hold on

plot(test4_node, test1_time(1)./test4_time)
hold on

grid minor
title('Speedup ./simpar-mpi 2 20 100000000 5','Interpreter', 'Latex');
ylabel('Speedup', 'Interpreter', 'Latex');
xlabel('Number of nodes used','Interpreter', 'Latex');
legend('1 core', '2 cores', '4 cores', 'Location', 'Best');

cell =  [10 20 30 40 50 60 70 90 100 250 500 1000];
time_cell = [1.037982  0.947220 1.001730 1.144673 1.148975 1.142921 ...
    1.160260 1.307613 1.261375 2.277810 6.566298 29.842344];

figure(1)
ax3 = subplot(2,2,3);
plot(cell, time_cell)
hold on

grid minor
title('Execution Time ./simpar-mpi 2 ncell 100000000 5','Interpreter', 'Latex');
ylabel('Execution Time [seconds]', 'Interpreter', 'Latex');
xlabel('Number of cells on the side of the grid','Interpreter', 'Latex');
legend('80x4 cores', 'Location','Best');

part = [100 1000 10000 100000 1000000 10000000 100000000 1000000000];
part_time = [0.040357  0.041093 0.042062 0.043754 0.047186 0.127850 ...
    0.950324 10.089763];

figure(1)
ax4 = subplot(2,2,4);
plot(part, part_time)
hold on

grid minor
title('Execution Time ./simpar-mpi 2 20 n\_part 5','Interpreter', 'Latex');
ylabel('Execution Time [seconds]', 'Interpreter', 'Latex');
xlabel('Number of particles','Interpreter', 'Latex');
set(gca, 'XScale', 'log');
legend('80x4 cores', 'Location','Best');




