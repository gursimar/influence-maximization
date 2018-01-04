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
	cout << "Nodes -> ";
	for (auto n : nodes){
		cout << n <<" ";
	} 
	cout << endl << "Edges" <<endl;

	vector<pair<int, int> > edges = xg.Edges();
	vector <double> probs = xg.Probabilities();
	vector <double> estimates = xg.Estimates();
	for (int i=0; i<xg.NumberOfEdges();i++){
		pair <int, int> e = edges[i];
		double prob = probs[i];
		double est = estimates[i];
		cout << e.first << "->" << e.second << " " << prob << " " << est << endl;
	} 
	cout << endl;

}

vector<int> runProbabilities(Xgraph &xg, int k, double epsilon, string model){
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

vector<int> runEstimates(Xgraph &xg, int k, double epsilon, string model){
    cout << " k:" << k << " epsilon:"<< epsilon <<   " model:" << model << endl;
	TimGraph m("","",xg.Edges(),xg.Estimates(),xg.NumberOfNodes());	
	
    m.k=k;
    if(model=="IC")
        m.setInfuModel(InfGraph::IC);
    else if(model=="LT")
        m.setInfuModel(InfGraph::LT);
    else
        ASSERT(false);

    cout<<"Finish Read Graph, Start Influence Maximization"<<endl;
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
		double m_eptim = 0.1;
		int m_budget;
		IC& m_ic;
		MAB(int budget, IC &ic):m_ic(ic) {
			m_budget = budget;
		}

		double expectedSpread(vector <int> seeds, int mc_iter, int environment) {
			int all_sp = 0;
			for (int i=0; i<mc_iter;i++) {
				int spread = m_ic.diffusion(seeds, environment);
				//cout <<spread <<endl;
				all_sp += spread;
				m_ic.m_graph.ResetAttributes();
			}
			//cout << "Seeds - " << seeds <<endl;
			//cout << "Expected spread - " << double(all_sp)/mc_iter <<endl;
			return double(all_sp)/mc_iter;
		}

		vector <int> explore(){
			//generate random seeds
			int noOfNodes = m_ic.m_graph.NumberOfNodes();
			vector<int> seeds_index = SWIFF_KNIFE::generateRandNo(noOfNodes,m_budget,1);
			vector<int> all_nodes = m_ic.m_graph.Nodes();

			vector<int> seeds;

			for(auto index: seeds_index) {
				//cout << index <<endl;
				seeds.push_back(all_nodes[index]);
			}
			return seeds;
		}

		vector<int> exploit(int environment){
			vector <int> seeds;
			if (environment==1) {
				seeds = runProbabilities(m_ic.m_graph, m_budget, m_eptim, "IC");	
			}
			else {
				seeds = runEstimates(m_ic.m_graph, m_budget, m_eptim, "IC");		
			}
			
			return seeds;
		}

		vector<int> epsilonGreedy() {
			vector <int> seeds;
			int environment = 0; // WE ALWAYS RUN EP_GREEDY ON ESTIMATES
			double random = double(rand())/ RAND_MAX;
			if (m_epsilon > random) {
				seeds = explore();
			}
			else {
				//exploit();
				seeds = exploit(environment);
			}

			return seeds;
		}

		void runMAB(int feedback) {
			if (feedback==1){
				//simple sharan's edge level feedback
				print_graph(m_ic.m_graph);
			}
			else if (feedback==2) {
				// expectation min edge feedback

			}
			else if (feedback ==3) {
				// expectation min node feedback

			}
			else {
				cout << "Feedback not supported";
			}

		}

};

double expectedSpreadGraph(Xgraph &xg, int mc_iter, int budget, int environment, double epsilon) {
	//cout <<seeds;
	vector <int> seeds = runProbabilities(xg, budget, epsilon, "IC");
	//vector <int> seeds = runEstimates(xg, budget, epsilon, "IC");
	IC ic(xg);
	int all_sp = 0;
	for (int i=0; i<mc_iter;i++) {
		int spread = ic.diffusion(seeds, environment);
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
	double epsilon = 0.1;
	
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
	

	//vector<int> simar =  SWIFF_KNIFE::range(0,9);
	//int a = ic.diffusion(seeds, 1);
	//	spreads.push_back(a);
	//}

	// COMPUTE EXPECTED SPREAD
	 int mc_iter = 100;
	 // int budget = 10;
	 // int environment = 1;
	 // double expSp = expectedSpreadGraph(xg, mc_iter, budget, environment, epsilon);//spreads);
	 // exit(0);

	// class MAB functions
	IC ic(xg);
	MAB mab(10, ic);
	int feedback = 1;
	// int environment = 1; // true probablities
	// vector <int> explore_seeds = mab.explore();
	// vector <int> exploit_seeds = mab.exploit(environment);
	// vector <int> eg_seeds = mab.epsilonGreedy();
	// cout << explore_seeds <<endl;
	// cout << exploit_seeds <<endl;
	// cout << eg_seeds <<endl;
	mab.runMAB(feedback);

	// double ex_spread = mab.expectedSpread(exploit_seeds, mc_iter, environment);
	// cout << "Expected Spread - " << ex_spread <<endl;

	cout << "\nEnded" << endl;



  return 0;
}

