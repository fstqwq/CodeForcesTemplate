mt19937 rnd((unsigned)(chrono::system_clock::now().time_since_epoch().count() / 100));
// 1e8 call : 374ms 
// mt19937_64 1e8 call : 951ms
// discard save half of time
shuffle(a.begin(), a.end(), rnd);
