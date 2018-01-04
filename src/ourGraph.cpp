using namespace std;
#include <iostream>
#include <vector>
#include <assert.h>
#include <queue>

#include "xgraph.cpp"
//#define HEAD_TRACE
#define HEAD_INFO

#define HEAD_INFO
//#define HEAD_TRACE
#include "sfmt/SFMT.h"
#include "head.h"
#include "memoryusage.h"
#include "graph.h"



class Node{
	int name;
	vector<int> predecessors;
	vector<int> successors;
};



template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}


class SWIFF_KNIFE {
public:
	static vector<int> generateRandNo(int n, int k, int flag) {
		vector<int> v;

		if (flag == 0) { //with replacement
			for (int i=0; i<k; i++){
			    int output;
			    output = (rand()%n)+1;
			    v.push_back(output);
			}
		}
		else {
			vector<int> nos = SWIFF_KNIFE::range(1,n);
			for (int i=0;i<k; i++){
				int index = (rand()%n);
				v.push_back(nos[index]);
				nos.erase(nos.begin() + index);
				n--;
			}
		}
		return v;
	}

	static vector<int> range(int start, int end){
		vector<int> v;
		for (int i=start; i<=end;i++){
			v.push_back(i);
		}
		return v;
	}

};

class IC{
	public:

		Xgraph& m_graph;
		IC(Xgraph &graph):m_graph(graph){
			//m_graph = graph;
		}

		int diffusion(vector<int> seeds, int isEnvironment) {
			queue<pair<int, int> > activeNodes;
			for(vector<int>::iterator seed = seeds.begin(); seed != seeds.end(); ++seed) {
				//cout << all_nodes.size()<<endl;
				//cout <<seed_node<<endl;

				// put 
				activeNodes.push(make_pair(*seed,0));
				//self.graph.node[seed]['isInfluenced'] = True
				//self.graph.node[seed]['influencedBy'] = -1
				//self.graph.node[seed]['influenceTimestep'] = 0
			}

			while (!activeNodes.empty()) {
				pair<int,int> curr = activeNodes.front();
				activeNodes.pop();
				int currentNode = curr.first;
				int timestep = curr.second;
				//cout << "Current node " << currentNode << " timestep " << timestep <<endl;
				for (auto & node : m_graph.Successors(currentNode)){
					//cout << node <<endl;
					
					if (!m_graph.IsInfluenced(node)){
						double dot;
						if (isEnvironment){
							dot = m_graph.Probability(currentNode, node);
						}
						else {
							dot = m_graph.Estimate(currentNode, node);
						}

						//cout << dot <<endl;
						double random = double(rand())/ RAND_MAX;
						if (dot > random) {
							activeNodes.push(make_pair(node,timestep+1));
							m_graph.SetIsInfluenced(node, currentNode, timestep+1);
						}

					}
					else {
					}
				}
			}
			return m_graph.getAllInfluencedNodes().size();
		}


};

class MAB {
	public:
		double m_epsilon = 0.5;
		int m_budget;
		int m_graph;
		MAB(int budget, int graph) {
			m_budget = budget;
			m_graph = graph;
		}

		void explore(){
			//generate random seeds
			vector<int> seeds = SWIFF_KNIFE::generateRandNo(100,10,1);
			//graph.Nodes and pick seeds

			/*
			for(vector<int>::iterator it = seeds.begin(); it != seeds.end(); ++it) {
				vector<int> all_nodes = m_graph.Nodes();
				//cout << all_nodes.size()<<endl;
				int seed_node = all_nodes[*it];
				//cout <<seed_node<<endl;

				// put 
				activeNodes.push(make_pair(seed_node,0));
				//self.graph.node[seed]['isInfluenced'] = True
				//self.graph.node[seed]['influencedBy'] = -1
				//self.graph.node[seed]['influenceTimestep'] = 0
			}
			*/
			

			//cout << seeds;
		}

		void exploit(){
			// seeds = TimGraph m(dataset, graph_file);
		}

		void epsilonGreedy() {
			double random = double(rand())/ RAND_MAX;
			if (m_epsilon > random) {
				explore();
			}
			else {
				exploit();
			}

		}

};

Xgraph readData(string m_filename, int m_n, int m_m){
	//cout << m_filename;
	Xgraph xg;
    FILE * fin= fopen((m_filename).c_str(), "r");

    //cout << fin;
    int readCnt=0;
    for(int i=0; i<m_m; i++){
        readCnt ++;
        int a, b;
        double p;
        int c=fscanf(fin, "%d%d%lf", &a, &b, &p);
        //cout << a << " " << b << " " << p << "\n";

        double random = double(rand())/ RAND_MAX;
		xg.AddEdge(a,b,p,random);
        //graph.AddEdge(a,b,p,random)
    }
    if(readCnt !=m_m)
        cout << "m not equal to the number of edges in file " + m_filename;
    fclose(fin);
    return xg;
}

void print_graph(Xgraph xg) {
	vector<int> nodes = xg.Nodes();
	for (auto n : nodes){
		cout << n <<" ";
	} 
	cout << endl;

	vector<pair<int, int> > edges;
	edges = xg.Edges();
	for (auto e : edges){
		cout << e.first <<"->" << e.second <<" ";
	} 
	cout << endl;

}

vector<int> run(Xgraph &xg, int k, double epsilon, string model ){
    cout << " k:" << k << " epsilon:"<< epsilon <<   " model:" << model << endl;
	TimGraph m("","",xg.Edges(),xg.Probabilities(),xg.NumberOfNodes());
    m.k=k;
    if(model=="IC")
        m.setInfuModel(InfGraph::IC);
    else if(model=="LT")
        m.setInfuModel(InfGraph::LT);
    else
        ASSERT(false);

    cout<<"Finish Read Graph, Start Influecne Maximization"<<endl;
    m.EstimateOPT(epsilon);
    cout<<"Time used: " << Timer::timeUsed[100]/TIMES_PER_SEC << "s" <<endl;

    //cout<<"Selected k SeedSet: ";
    //for(auto item:m.seedSet)
    //    cout<< item << " ";
    //cout<<endl;
    //cout<<"Estimated Influence: " << m.InfluenceHyperGraph() << endl;
    Counter::show();

    return m.seedSet;
}

double expectedSpread(Xgraph &xg, int mc_iter, int budget) {
	//cout <<seeds;
	vector <int> seeds = run(xg, budget, 0.1, "IC");
	IC ic(xg);
	int all_sp = 0;
	for (int i=0; i<mc_iter;i++) {
		int spread = ic.diffusion(seeds, 1);
		//cout <<spread <<endl;
		all_sp += spread;
		xg.ResetAttributes();
	}
	cout << "Seeds - " << seeds <<endl;
	cout << "Expected spread - " << double(all_sp)/mc_iter <<endl;


	return double(all_sp)/mc_iter;
}

int main()
{
	srand((unsigned)time(0));
	
	cout << "\nStarted\n" << endl;
	// int m_n = 15229;
	// int m_m = 62752;
	// Xgraph xg = readData("nethept/graph_ic.inf", m_n, m_m);

	int m_n = 500;
	int m_m = 2509;
	Xgraph xg = readData("nethept/synth_500.txt", m_n, m_m);

	// play area
	//print_graph(xg);
	//cout << xg.Probabilities();
	//vector <int> seeds = run(xg, 50, 0.1, "IC");
	//cout <<seeds;
	
	//IC ic(xg);

	//vector<int> simar =  SWIFF_KNIFE::range(0,9);
	//int a = ic.diffusion(seeds, 1);
	//	spreads.push_back(a);
	//}

	int mc_iter = 100;
	int budget = 10;
	double expSp = expectedSpread(xg, mc_iter, budget);//spreads);

	// class MAB functions
	//MAB mab(20,20);
	//mab.explore();
	

	cout << "\nEnded" << endl;



  return 0;
}

