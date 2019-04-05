function tests = join_data()

    test_n = zeros(7, 12, 5);
    teste_1(); test_n(1, : ,:) = test_1;
    teste_2(); test_n(2, : ,:) = test_2;
    teste_3(); test_n(3, : ,:) = test_3;
    teste_4(); test_n(4, : ,:) = test_4;
    teste_5(); test_n(5, : ,:) = test_5;
    teste_6(); test_n(6, : ,:) = test_6;
    teste_7(); test_n(7, : ,:) = test_7;
    
    test_single = struct( ...
        'threads', zeros(12, 1), ...
        'real', zeros(12, 1), ...
        'sys', zeros(12, 1), ...
        'cpu', zeros(12, 1), ...
        'speedup', zeros(12, 1));
    
    tests = repmat(test_single, 7, 1);
   
    for n = 1:7
        tests(n).threads = test_n(n, :, 1)';
        tests(n).real    = test_n(n, :, 2)';
        tests(n).sys     = test_n(n, :, 3)';
        tests(n).cpu     = test_n(n, :, 4)';
        tests(n).speedup = ...
            (tests(n).real(1) ./ tests(n).real) ./ tests(n).threads * 100;
    end

end