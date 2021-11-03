#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iomanip>

#define N 100
#define BOUND 2
using namespace std;

class ItemSet
{
public:
  int t_size, frequency;
  char items[N];

  ItemSet()
  {
    frequency = 0;
    t_size = 0;
  }

  ItemSet(char items[])
  {
    frequency = 0;
    t_size = strlen(items);
    memcpy(this->items, items, t_size);
  }
  void setter(int t_id, vector<char> items)
  {
    t_size = items.size();
    for (int i = 0; i < items.size(); i++)
      this->items[i] = items[i];
  }
  void printItems()
  {
    cout << "( ";
    for (int i = 0; i < t_size; i++)
    {
      cout << items[i];
      if (i + 1 < t_size)
        cout << ", ";
    }
    cout << " )";
  }
  void add(char item)
  {
    items[t_size] = item;
    t_size++;
  }
  void remove(int index)
  {
    for (int i = index + 1; i < t_size; i++)
    {
      items[i - 1] = items[i];
    }
    t_size--;
  }
  void increase_count()
  {
    frequency++;
  }

  bool isSubset(char subItems[], int size)
  {

    // checking for all characters of Itemsets
    for (int i = 0; i < t_size; i++)
    {
      bool found = false;
      // checking for each character of Transaction
      for (int j = 0; j < size; j++)
      {
        if (items[i] == subItems[j])
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        return false;
      }
    }
    increase_count();
    return true;
  }
};

class Transaction
{

public:
  int t_id, t_size;
  char items[N];

  Transaction() {}

  Transaction(int id, char items[])
  {
    t_id = id;
    t_size = strlen(items);
    memcpy(this->items, items, t_size);
  }
  void setter(int id, vector<char> items)
  {
    t_id = id;
    t_size = items.size();
    for (int i = 0; i < items.size(); i++)
      this->items[i] = items[i];
  }

  // result : is to keep result
  // k : length of subset of items in Transaction of length s
  // s : number of items in subset
  // currLen : keep current length
  // mark : it keep marking items till length k
  vector<ItemSet> combinations(vector<ItemSet> result, int k, int s, int currLen, bool mark[])
  {
    if (currLen > k)
      return result;
    else if (currLen == k)
    {
      ItemSet itemset;
      // vector<char> itemset;
      for (int i = 0; i < t_size; i++)
      {
        if (mark[i] == true)
        {
          itemset.add(items[i]);
        }
      }
      result.push_back(itemset);
      return result;
    }
    if (s == t_size)
      return result;
    mark[s] = true;
    result = this->combinations(result, k, s + 1, currLen + 1, mark);
    mark[s] = false;
    result = this->combinations(result, k, s + 1, currLen, mark);
    return result;
  }
};

int main()
{
  int n = 11;
  bool mark[n] = {false};

  // All Transaction Data
  // vector<char> data[10] = {
  //     {'A', 'C', 'F'},
  //     {'D', 'G'},
  //     {'A', 'B', 'E', 'G'},
  //     {'A', 'B', 'F'},
  //     {'C', 'F', 'G'},
  //     {'A', 'C', 'G'},
  //     {'C', 'D'},
  //     {'A', 'B', 'D', 'F'},
  //     {'A', 'B', 'C', 'D', 'F'},
  //     {'C', 'D', 'F'}};
  vector<char> data[10] = {
      {'A', 'B', 'C', 'D'},
      {'A', 'B', 'C', 'D', 'E', 'G'},
      {'A', 'C', 'G', 'H', 'K'},
      {'B', 'C', 'D', 'E', 'K'},
      {'D', 'E', 'F', 'H', 'L'},
      {'A', 'B', 'C', 'D', 'L'},
      {'B', 'I', 'E', 'K', 'L'},
      {'A', 'B', 'D', 'E', 'K'},
      {'A', 'E', 'F', 'H', 'L'},
      {'B', 'C', 'D', 'F'}};

  // Transaction Table
  Transaction t[10];
  int ts_size = 10;
  for (int i = 0; i < 10; i++)
  {
    t[i].setter(i, data[i]);
  }

  // All items in transaction
  char totalSets[n] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L'};
  Transaction completeSet(100, totalSets);

#pragma omp parallel num_threads(3) shared(t, ts_size)
  {
    // int terminal = system("konsole");
    vector<ItemSet> i_itemset;
    int tid = omp_get_thread_num();
#pragma omp critical
    cout << "Thread " << tid << " solving for " << tid + 1 << "-itemset" << endl;

    // Finding all combinations of i_itemset
    i_itemset = completeSet.combinations(i_itemset, tid + 1, 0, 0, mark);

    // Comparing to Find Frequency using subset
    for (int i = 0; i < i_itemset.size(); i++)
    {
      for (int j = 0; j < ts_size; j++)
      {
        i_itemset[i].isSubset(t[j].items, t[j].t_size);
      }
    }

    // Removing the ones with less frequency than BOUND
    for (int k = 0; k < i_itemset.size(); k += 1)
    {
      if (i_itemset[k].frequency < BOUND)
      {
        i_itemset.erase(i_itemset.begin() + k--);
      }
    }

// Printing Result
#pragma omp critical
    {

      for (int l = 0; l < i_itemset.size(); l++)
      {
        // cout<< "Thread: "<<omp_get_thread_num() << " ===> " ;
        i_itemset[l].printItems();
        cout << " Frequency = " << i_itemset[l].frequency << endl;
      }
    }
  }

  return 0;
}
