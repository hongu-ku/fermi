#include <bits/stdc++.h>

#define size_t unsigned long long
#define ll long long
using namespace std;

typedef pair<int, int> P;

class State
{
public:
  vector<int> v; // それぞれの粒子のエネルギー
  int score;     // 総エネルギー数
  int num;       // 粒子数

  State(int n)
  {
    v = vector<int>(n);
    score = 0;
    num = n;
    for (int i = 0; i < n; i++)
    {
      v[i] = i;
      score += i;
    }
  }

  State(vector<int> input)
  {
    num = input.size();
    score = 0;
    v = input;
    for (int i = 0; i < num; i++)
    {
      score += input[i];
    }
  }

  void filePrint()
  {
    ofstream outputfile("state_list.txt");
    outputfile << score << " : [";
    for (int i = 0; i < size(); i++)
    {
      outputfile << v[i] << ", ";
    }
    outputfile << "\b\b]" << endl;
    outputfile.close();
  }

  void print()
  {
    cout << score << " : [";
    for (int i = 0; i < size(); i++)
    {
      cout << v[i] << ", ";
    }
    cout << "\b\b]" << endl;
  }

  bool increment(int idx)
  {
    if (this->size() <= idx)
    {
      cout << "State: idx(" << idx << ") is bigger than size(" << size() << ")" << endl;
      return false;
    }
    this->v[idx]++;
    return true;
  }

  bool decrement(int idx)
  {
    if (this->size() <= idx)
    {
      cout << "State: idx is bigger than size()" << endl;
      return false;
    }
    this->v[idx]--;
    return true;
  }

  int size()
  {
    return num;
  }
};

bool operator==(const State &a, const State &b)
{
  if (b.v.size() != a.v.size())
    return false;
  for (int i = 0; i < b.v.size(); i++)
  {
    if (b.v[i] != a.v[i])
      return false;
  }
  return true;
}

bool operator!=(const State &a, const State &b)
{
  return !(a == b);
}

bool operator<(const State &a, const State &b)
{
  if (a.score == b.score)
  {
    for (int i = 0; i < b.v.size(); i++)
    {
      if (a.v[i] != b.v[i])
        return (a.v[i] < b.v[i]);
    }
  }
  return a.score < b.score;
}

bool operator>(const State &a, const State &b) { return b < a; }
bool operator<=(const State &t1, const State &t2) { return !(t1 > t2); }
bool operator>=(const State &t1, const State &t2) { return !(t1 < t2); }

class Fermi
{
  set<State> s;  // 各ステートを管理
  int lim_score; // 総エネルギーの最大値
  int n;         // 粒子数

public:
  Fermi(int n, int lim_size = 10)
  {
    //O()
    init(n, lim_size);
  }

  Fermi()
  {
    init(3);
  }

  void printAll()
  {
    cout << "ε : state" << endl;
    for (auto state : s)
    {
      state.filePrint();
      // state.print();
    }
  }

  void fileCountPrint()
  {
    map<int, int> m;
    for (auto state : s)
    {
      m[state.score]++;
    }
    ofstream outputfile("state_count" + to_string(n) + ".csv");
    outputfile << "level,nums" << endl;
    for (auto ma : m)
    {
      outputfile << ma.first << ", " << ma.second << endl;
    }
    outputfile.close();
  }

  void countPair() // O(準位数 * N * lim_size)
  {
    vector<vector<int>> pair(lim_score + 1, vector<int>(lim_score + 1, 0));
    int num = 0;
    vector<vector<int>> diff(n - 1, vector<int>(n - 1));
    bool flag = true;
    for (auto a : s)
    {
      if (num != a.score)
      {
        num = a.score;
      }
      for (int i = 0; i < n - 1; i++)
      {
        for (int j = i + 1; j < n; j++)
        {
          diff[i][j] = a.v[j] - a.v[i];
          if (flag)
            cout << diff[i][j] << endl;
        }
      }
      flag = false;

      for (int i = 1; i + a.score <= lim_score; i++)
      {
        pair[a.score][a.score + i] += n;
      }

      for (int i = 0; i < n - 1; i++)
      {
        for (int j = i + 1; j < n; j++)
        {
          if (a.score + diff[i][j] <= lim_score)
            pair[a.score][a.score + diff[i][j]]--;
        }
      }
    }

    ofstream outputfile("pair_count" + to_string(n) + ".csv");
    for (int i = 0; i < pair.size(); i++)
    {
      if (i == 0)
      {
        for (int j = 0; j < pair[i].size(); j++)
          outputfile << j << ",";
        outputfile << "\b" << endl;
      }
      for (auto p : pair[i])
      {
        outputfile << p << ", ";
      }
      outputfile << "\b\b" << endl;
    }
    outputfile.close();
    return;
  }

  void secondPair()
  {
    cout << "secondPair" << endl;
    int lim = lim_score + 1;
    int num = 0;
    int pairs[lim][lim][8];
    vector<string> strs{"+++", "++-", "+-+", "+--", "-++", "-+-", "--+", "---"};

    ofstream outputfile("pair3D2_count" + to_string(n) + ".csv");
    for (auto str : strs)
      outputfile << str << ",";
    outputfile << "\n";
    for (auto ite1 = s.begin(); ite1 != s.end(); ++ite1)
    {
      if (num != ite1->score)
      {
        for (int i = 0; i < lim; i++)
        {
          for (int j = 0; j < lim; j++)
          {
            for (int l = 0; l < 8; l++)
            {
              pairs[i][j][l] = 0;
            }
          }
          num = ite1->score;
          cout << num << " / " << lim << endl;
        }
        for (auto ite2 = next(s.end(), -1); ite2 != ite1; --ite2)
        {
          if (ite1->score == ite2->score)
            break;
          unordered_map<int, int> temp;
          for (int i = 0; i < n; i++)
          {
            temp[ite1->v[i]]++;
            temp[ite2->v[i]]++;
          }
          for (auto ite3 = next(s.end(), -1); ite3 != ite2; --ite3)
          {
            if (ite2->score == ite3->score)
              break;
            int idx = (int)temp.size() != n + 1 ? 4 : 0;
            unordered_map<int, int> temp1, temp2;
            for (int i = 0; i < n; i++)
            {
              temp1[ite3->v[i]]++;
              temp1[ite2->v[i]]++;
              temp2[ite1->v[i]]++;
              temp2[ite3->v[i]]++;
            }
            idx += (int)temp1.size() != n + 1 ? 2 : 0;
            idx += (int)temp2.size() != n + 1 ? 1 : 0;
            pairs[ite2->score][ite3->score][idx]++;
          }
        }
        if (next(ite1, 1) == s.end() || ite1->score != next(ite1, 1)->score)
        {
          for (int j = ite1->score + 1; j < lim; j++)
          {
            for (int k = j + 1; k < lim; k++)
            {
              outputfile << ite1->score << "," << j << "," << k << ",";
              for (int l = 0; l < 8; l++)
              {
                outputfile << pairs[j][k][l] << ",";
              }
              outputfile << "\n";
            }
          }
        }
      }

      outputfile.close();
    }
  }

  void makePair()
  {
    vector<vector<int>> pairVec(lim_score + 1, vector<int>(lim_score + 1, 0));
    int num = 0;
    for (auto a : s)
    {
      if (num != a.score)
      {
        cout << num << " / " << lim_score << endl;
        num = a.score;
      }
      for (auto b : s)
      {
        if (a.score <= b.score)
        {
          continue;
        }
        if (connected(a, b))
        {
          pairVec[a.score][b.score]++;
        }
      }
    }
    ofstream outputfile("pairVec_count" + to_string(n) + ".csv");
    for (int i = 0; i < pairVec.size(); i++)
    {
      if (i == 0)
      {
        for (int j = 0; j < pairVec[i].size(); j++)
          outputfile << j << ",";
        outputfile << "\b" << endl;
      }
      for (auto p : pairVec[i])
      {
        outputfile << p << ", ";
      }
      outputfile << "\b\b" << endl;
    }
    outputfile.close();
    return;
  }

private:
  void init(int n, int lim_size = 10)
  {
    this->n = n;
    State state(n);
    lim_score = state.score + lim_size;
    init(state.v);
  }

  /// 深さ優先探索でステートを列挙
  void init(const vector<int> &v)
  {
    State state(v);
    if (!array_is_unique(v))
      return;
    if (state.score > lim_score)
      return;
    if (s.emplace(state).second)
    {
      for (int i = 0; i < state.size(); i++)
      {
        state.increment(i);
        init(state.v);
        state.decrement(i);
      }
    };
  }

  bool array_is_unique(const vector<int> &v)
  {
    for (int i = 0; i < v.size() - 1; i++)
    {
      if (v[i] == v[i + 1])
        return false;
    }

    return true;
  }

  bool connected(const State &a, const State &b)
  {
    /*
    024
    034
    */
    int i = 0, j = 0;
    int diff = 0;
    while (i < a.num && j < b.num)
    {
      if (a.v[i] == b.v[i])
      {
        i++;
        j++;
      }
      else if (a.v[i] < b.v[j])
      {
        if (++diff > 1)
          return false;
        i++;
        if (a.v[i] > b.v[i])
          j++;
      }
      else
      {
        if (++diff > 1)
          return false;
        j++;
        if (a.v[i] > b.v[i])
          j++;
      }
    }
    return true;
  }
};

int main(int argc, char **argv)
{
  int n, lim_size;
  if (argc > 1)
  {
    n = atoi(argv[1]);
    lim_size = atoi(argv[2]);
  }
  else
  {
    cout << "粒子数nを入力" << endl;
    cin >> n;
    cout << "基底準位からのエネルギーの上がり幅lim_sizeを入力" << endl;
    cin >> lim_size;
  }
  Fermi f(n, lim_size);
  cout << "aaa" << endl;
  f.secondPair();
  // f.fileCountPrint();
}
