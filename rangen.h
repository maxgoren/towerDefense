using namespace std;
int getrand(int a, int b)
{
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> distrib(a,b); 
  return distrib(gen);
}