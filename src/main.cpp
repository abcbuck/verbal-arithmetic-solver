#include <algorithm> //for_each, max_element
#include <iostream>
#include <set>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector> //make_pair

bool lengthLess(std::string& a, std::string& b) {
  return a.length() < b.length();
}

//quit program if false is returned; a true return value doesn't affect program flow
bool backtrack(
  int numberOfSummands,
  std::string& sum,
  std::unordered_map<char, int>& resolveChar,
  std::vector<std::vector<char>>& columns,
  std::set<int>& digitsAvailable,
  std::stack<std::tuple<int, int>>& memory,
  int& column,
  int& row,
  std::stack<int>& carries,
  int& lastDigit
) {
  for(std::tuple<int, int> memtop;;) {
    if(memory.empty())
      //brute-force complete, quit program
      return false;
    memtop = memory.top();
    memory.pop();
    column = std::get<0>(memtop);
    row = std::get<1>(memtop);
    if(row != numberOfSummands)
      break;
    digitsAvailable.insert(resolveChar[sum[sum.size()-1-column]]);
    resolveChar[sum[sum.size()-1-column]] = -1;
  }
  lastDigit = resolveChar[columns[column][row]];
  digitsAvailable.insert(lastDigit);
  resolveChar[columns[column][row]] = -1;
  while(carries.size()-1 > column)
    carries.pop();
  return true;
}

int main() {
  //input
  int numberOfSummands;
  std::cout << "Please enter the number of summands: ";
  std::cin >> numberOfSummands;

  std::vector<std::string> summands(numberOfSummands);
  std::string sum;
  std::cout << "Please enter summands and sum:" << std::endl;
  for(int i = 0; i < numberOfSummands; i++)
    std::cin >> summands[i];
  std::cin >> sum;

  if(std::max_element(summands.begin(), summands.end(), lengthLess)->size() > sum.length()) {
    std::cout << "Can't solve problem: Summands are longer than sum.";
    return 0;
  }
  //input end

  //create map of chars and digits
  std::unordered_map<char, int> resolveChar;
  for(int i = 0; i < numberOfSummands; i++)
    for(int j = 0; j < summands[i].length(); j++)
      resolveChar.emplace(std::make_pair(summands[i][j], -1));
  for(int i = 0; i < sum.length(); i++)
    resolveChar.emplace(std::make_pair(sum[i], -1));
  
  if(resolveChar.size() > 10) {
    std::cout << "Can't solve problem: There are more than 10 variables.";
    return 0;
  }

  resolveChar.emplace(std::make_pair<char, int>(0, 0)); //leading spaces, in case sum is longer than any of the summands, take the value zero

  //store summands column-wise, indexed from right to left (little-endian)
  std::vector<std::vector<char>> columns(sum.length(), std::vector<char>(numberOfSummands));
  for(int i = 0; i < numberOfSummands; i++) {
    int j = 0;
    while(j < summands[i].length()) {
      columns[j][i] = summands[i][summands[i].length()-1-j];
      j++;
    }
    while(j < sum.length()) {
      columns[j][i] = 0;
      j++;
    }
  }

  std::set<int> digitsAvailable = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  //stack of positions (int, int) in the addition table at which numbers are tried as values for characters
  //the stack is used to backtrack in case a combination of values didn't work out while brute-forcing
  std::stack<std::tuple<int, int>> memory;

  int column = 0, row = 0;
  std::stack<int> carries;
  carries.push(0); //must be 0 to start with!
  int lastDigit = -1;
  int counter = 0;
  for(;;) {
    //all characters are mapped to digits and all chars match: output a solution if one was found, then backtrack
    if(column == sum.length()) {
      //if all summands add up without undue carry, a solution was found (9+9=8 is not a solution, because the carry is 1)
      if(carries.top() == 0) {//output
        std::cout << std::endl << "Counter: " << ++counter << std::endl;
        std::for_each(resolveChar.begin(), resolveChar.end(), [](std::pair<const char, int>& el) {
          std::cout << el.first << " = " << el.second << ", ";
        });
        std::cout << std::endl;
        for(int i = 0; i < summands.size(); i++) {
          for(int j = summands[i].length(); j < sum.length(); j++)
            std::cout << " ";
          std::cout << summands[i] << "    ";
          for(int j = summands[i].length(); j < sum.length(); j++)
            std::cout << " ";
          for(int j = 0; j < summands[i].length(); j++)
            std::cout << resolveChar[summands[i][j]];
          std::cout << std::endl;
        }
        std::cout << sum << "    ";
        for(int j = 0; j < sum.length(); j++)
          std::cout << resolveChar[sum[j]];
        std::cout << std::endl;
      }//output

      backtrack(numberOfSummands, sum, resolveChar, columns, digitsAvailable, memory, column, row, carries, lastDigit);
      continue;
    }
    //unmapped character at the current position
    if(resolveChar[columns[column][row]] == -1) {
      //if the digit that last was tried for the character at the current position wasn't the highest left, try the next highest one
      if(lastDigit != -1) {
        std::set<int>::iterator it = digitsAvailable.find(lastDigit);
        //=> it != digitsAvailable.end(), because lastDigit != -1 only if backtracking has occurred, and if backtracking has occurred, lastDigit is in digitsAvailable
        if(++it != digitsAvailable.end()) {
          resolveChar[columns[column][row]] = *it;
          digitsAvailable.erase(it);
          memory.push(std::make_tuple(column, row));
          lastDigit = -1;
        }
        else {
          if(!backtrack(numberOfSummands, sum, resolveChar, columns, digitsAvailable, memory, column, row, carries, lastDigit))
            return 0;
          continue;
        }
      }
      else {
        resolveChar[columns[column][row]] = *(digitsAvailable.begin());
        digitsAvailable.erase(digitsAvailable.begin());
        memory.push(std::make_tuple(column, row));
      }
    }
    if(row < numberOfSummands-1)
      //advance row
      row++;
    else { //(when at the end of the row)
      //def/check sum and go to next column
      int sumIndex = sum.size()-1-column;
      int columnSum = carries.top();
      for(int i = 0; i < numberOfSummands; i++)
        columnSum += resolveChar[columns[column][i]];
      if(resolveChar[sum[sumIndex]] == -1 && digitsAvailable.find(columnSum % 10) != digitsAvailable.end()) {
        resolveChar[sum[sumIndex]] = columnSum % 10;
        digitsAvailable.erase(columnSum % 10);
        memory.push(std::make_tuple(column, numberOfSummands));
      }
      if(resolveChar[sum[sumIndex]] == columnSum % 10) {
        column++;
        row = 0;
        carries.push(columnSum/10);
      }
      else {
        backtrack(numberOfSummands, sum, resolveChar, columns, digitsAvailable, memory, column, row, carries, lastDigit);
        //continue;
      }
    }
  }//for(;;)

  return 0;
}
