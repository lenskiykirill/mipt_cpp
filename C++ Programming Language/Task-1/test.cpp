#include <iostream>
#include <cassert>
#include <type_traits>
#include <sstream>

#include "string.h"

void test1 () {
      String s("abcdef");

      assert(s.length() == 6);

      assert(s[0] == 'a');
      assert(s[3] == 'd');
      assert(s[5] == 'f');

      assert(s.back() == 'f');
      assert(s.front() == 'a');

      s[1] = 'x';
      s.back() = 'y';
      s.front() = 'y';

      assert(s[0] == 'y');
      assert(s[5] == 'y');
      assert(s[1] == 'x');

      std::cout << "Test 1 passed\n";
}

void test2 () {
      const String s ("abcdef");

      assert(s.length() == 6);

      assert(s[0] == 'a');
      assert(s[3] == 'd');
      assert(s[5] == 'f');

      assert(s.back() == 'f');
      assert(s.front() == 'a');

      assert((!std::is_assignable_v<decltype(static_cast<const String&>(String())[0]), char>));
      assert((!std::is_assignable_v<decltype(static_cast<const String&>(String()).front()), char>));
      assert((!std::is_assignable_v<decltype(static_cast<const String&>(String()).back()), char>));

      std::cout << "Test 2 passed\n";
}

void test3 () {
      String s = "aaa";
      
      for (int i = 0; i < 500000; i++) {
            s.push_back ('a');
      }
      assert (s.length() == 500003);
      assert (s[500000] == 'a');

      String ss = s;
      ss[500000] = 'b';

      assert (s.length() == 500003);
      assert (s[500000] == 'a');

      for (int i=0; i < 500000; i++) {
            s.pop_back();
      }


      assert (s.length() == 3);
      assert (ss.length() == 500003);

      ss = s;
      s = String ("abc");

      assert (ss.length() == 3);
      assert (ss[2] == 'a');

      const String sss(1000, 'a');
      
      String sz = sss;

      for (int i=0; i < 500000; i++) {
            s += sz.back();
      }

      s = s;

      assert (s.length() == 500003);
      assert (s[500002] == 'a');
      assert (s[266532] == 'a');

      (s += String(100, 'b')) += String(100, 'c');

      assert (s.length() == 500203);
      assert (s.back() == 'c');

      for (int i=0; i < 100; i++) {
            s.pop_back();
      }

      assert (s.back() == 'b');

      s = ss = s;
      (ss = s) = ss;

      std::cout << "Test 3 passed\n";
}

void test4 () {
      String s;
      
      std::istringstream iss;
      iss.str ("1234567 12345");
      iss >> s;
      
      assert(s.length() == 7);

      String ss;     
      iss >> ss;

      assert(!ss.empty());
      assert(ss.length() == 5);

      assert((s + ss).length() == 12);

      auto sum = ss + s;

      assert(ss.length() == 5);
      assert(s.length() == 7);
      assert(sum.length() == 12);

      std::cout << "Test 4 passed\n";
}

void test5 () {
      String s = "abcde";
      
      {
            auto& ss = s+= 'f';
            ss += 'g';
      }
      
      assert(s.length() == 7);
      
      {
            auto& ss = (s = s);
            assert(ss[5] == 'f');
            
            auto sss = ss;

            ss.clear();
            
            assert(sss.length() == 7);
      }

      std::cout << "Test 5 passed\n";
}

void test6 () {
      const String s = "122333444455555";

      assert(s.find("44") == 6);
      assert(s.rfind("44") == 8);
      
      assert(s.find("12345") == (std::is_same_v<String, std::string> ? std::string::npos : s.length()));
      
      assert(s.substr(5, 4) == "3444");

      decltype(s.substr(1,1)) ss = s;
      ss[0] = '0';

      assert(s[0] == '1');

      {
            auto& sss = ss;
            sss[0] = '2';
      }

      assert(ss[0] == '2');

      {
            auto& ssss = ss;
            ssss.front() = '3';
      }
      assert (ss.front() == '3');

      std::cout << "Test 6 passed\n";
}

int main () {

      test1();
      
      test2();
      
      test3();
      
      test4();
      
      test5();

      test6();

      return 0;
}
