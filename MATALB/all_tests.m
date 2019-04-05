close all
tests = join_data();

for n = 1:7

   figure(1)
   plot(tests(n).threads , tests(n).real)
   hold on

   figure(2)
   plot(tests(n).threads , tests(n).speedup)
   hold on
   
   figure(3)
   plot(tests(n).threads , tests(n).speedup.*tests(n).threads/100)
   hold on
end

figure(1)
plot(tests(4).threads , tests(4).real)
hold on

figure(1)
legend(...
    "1 3   1000000     20",...
    "1 10 2000000     10", ...
    "1 30 20000000   10", ...
    "2 5   10000         10", ...
    "4 20 100000       5", ...
    "8 5   1000000     50", ...
    "2 20 100000000 5", 'Location', 'Best');
xlim([1 12]);
grid minor
title("Execution time of ./simpar\_omp <data>")
xlabel("# of Threads")
ylabel("Execution Time [sec] ");
set(gca, 'YScale', 'log')

figure(2)
legend(...
    "1 3   1000000     20",...
    "1 10 2000000     10", ...
    "1 30 20000000   10", ...
    "2 5   10000         10", ...
    "4 20 100000       5", ...
    "8 5   1000000     50", ...
    "2 20 100000000 5", 'Location', 'Best');
xlim([1 12]);
grid minor
title("Efficiency ./simpar\_omp <data>")
xlabel("# of Threads")
ylabel("Speedup / # of Threads [%] ");
        

figure(3)
legend(...
    "1 3   1000000     20",...
    "1 10 2000000     10", ...
    "1 30 20000000   10", ...
    "2 5   10000         10", ...
    "4 20 100000       5", ...
    "8 5   1000000     50", ...
    "2 20 100000000 5", 'Location', 'Best');
xlim([1 12]);
grid minor
title("Speedup ./simpar\_omp <data>")
xlabel("# of Threads")
ylabel("Speedup");
        
        
        