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

        double random = double(rand())/ (RAND_MAX);
		xg.AddEdge(a,b,p,random);
		//xg.AddEdge(a,b,p,0);
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
	vector <int> shTriggeredCounts = xg.sharanTriggeredCounts();
	vector <int> shActivatedCounts = xg.sharanActivatedCounts();

	int total_cnt1 =0;
	int total_cnt2 =0;

	for (int i=0; i<xg.NumberOfEdges();i++){
		pair <int, int> e = edges[i];
		double prob = probs[i];
		double est = estimates[i];
		//int cnt1 = shTriggeredCounts[i];
		//int cnt2 = shActivatedCounts[i];
		//total_cnt1 += cnt1;
		//total_cnt2 += cnt2;
		int eta = xg.getETA(e.first,e.second);
		int isInf = xg.isInfluencedEdge(e.first, e.second);
		//cout << e.first << "->" << e.second << " " << prob << " " << est << endl;
		//cout << e.first << "->" << e.second << " " << prob << " " << est << " " << cnt1 <<" " << cnt2 << endl;
		cout << e.first << "->" << e.second << " " << prob << " " << est << " " << eta <<" " << isInf << endl;
	} 
	//cout << "Triggered - " << total_cnt1 << " Actiavted - " << total_cnt2;
	cout << endl;
}

void print_nodes(Xgraph xg, int onlyInf) {
	if (onlyInf == 0) {
		vector<int> nodes = xg.Nodes();
		cout << endl << "Nodes -> ";
		cout << endl << "NodeID IsInfluenced InfluencedBy Timestep" <<endl;
		for (auto n : nodes){
			cout << n <<" " << xg.IsInfluenced(n) << " " << xg.InfluencedBy(n) << " " << xg.Timestep(n) << " " <<endl;
		} 
	}
	else {
		set<int> nodes = xg.getAllInfluencedNodes();
		cout << endl<< "Influenced Nodes -> ";
		cout << endl << "NodeID IsInfluenced InfluencedBy Timestep" <<endl;
		for (auto n : nodes){
			cout << n <<" " << xg.IsInfluenced(n) << " " << xg.InfluencedBy(n) << " " << xg.Timestep(n) << " " <<endl;
		}

	}
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
			vector<int> nos = SWIFF_KNIFE::range(0,n-1);
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
				m_graph.SetIsInfluencedNode(*seed, 0); // add seeds to influenced
				//self.graph.node[seed]['isInfluenced'] = True
				//self.graph.node[seed]['influencedBy'] = -1
				//self.graph.node[seed]['influenceTimestep'] = 0
			}

			vector <double> nos;

			while (!activeNodes.empty()) {
				pair<int,int> curr = activeNodes.front();
				activeNodes.pop();
				int currentNode = curr.first;
				int timestep = curr.second;
				//cout <<timestep;
				//exit(0);
				m_graph.SetIsInfluencedNode(currentNode, timestep); // add seeds to influenced
				//cout << "Current node " << currentNode << " timestep " << timestep <<endl;
				for (auto & node : m_graph.Successors(currentNode)){
					//cout << node <<endl;
					m_graph.triggerEdge(currentNode,node);
					if (!m_graph.IsInfluenced(node)){
						//m_graph.triggerEdge(currentNode,node); NOT SURE WHERE TO PUT THIS
						double dot;
						if (isEnvironment){
							dot = m_graph.Probability(currentNode, node);
							//cout <<  "p";
						}
						else {
							dot = m_graph.Estimate(currentNode, node);
							//cout <<  "e";
						}

						//cout << dot <<endl;
						double random = double(rand())/ RAND_MAX;
						nos.push_back(random);
						//cout << random <<endl;
						if (dot > random) {
							activeNodes.push(make_pair(node,timestep+1));
							m_graph.activateEdge(currentNode,node); // this is for sharan_edge
							m_graph.SetIsInfluenced(node, currentNode, timestep+1);
						}

					}
					else {
					}
				}
			}
			//cout << nos;
			//exit(0);
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

		void runMAB(int feedback, int iterations) {
			if (feedback==1){
				//simple sharan's edge level feedback
				sharanEdgeFeedback(iterations);
			}
			else if (feedback==2) {
				// expectation min edge feedback
				SidNodeLevel(iterations);

			}
			else if (feedback ==3) {
				// expectation min node feedback
				SidEdgeLevel(iterations);

			}
			else {
				cout << "Feedback not supported";
			}
		}

		void SidEdgeLevel(int iterations){
			int environment = 1;
			for (int i=0;i<iterations;i++){
				//cout << "L1 Error is - " << computeL1() << endl;
				// Do bandit round
				vector<int> seeds = explore();
				int spread = m_ic.diffusion(seeds, environment);
				//cout << "Explore seeds " << seeds <<endl;
				//cout << "Spread - " << spread <<endl;
				//exit(0);

				// UPDATE STEP
				// for each activated node, compute pr of activation
				set<int> allInfNodes = m_ic.m_graph.getAllInfluencedNodes();
				for (auto infNode: allInfNodes ) {
					int infNodeInd = m_ic.m_graph.getNodeIndex(infNode);
					int timeNode = m_ic.m_graph.Timestep(infNode);
					//cout << infNode << "->" << timeNode <<  " | ";

					vector<int> preds = m_ic.m_graph.Predecessors(infNode);
					for (auto pred: preds){
						double x_uv = 0.0;
						if (m_ic.m_graph.IsInfluenced(pred)==1 && m_ic.m_graph.Timestep(pred)<timeNode && m_ic.m_graph.Timestep(pred)>=0){
						//if (m_ic.m_graph.IsInfluenced(pred) ==1 && m_ic.m_graph.Timestep(pred)>=0){
							// set S+ U S-
							m_ic.m_graph.triggerETA(pred,infNode);
							if (m_ic.m_graph.InfluencedBy(infNode)==pred) {
								// (S+) active parents of infNode at time t-1
								assert(m_ic.m_graph.Timestep(pred)==timeNode-1);
								x_uv = 1.0;

							}
							else {
								x_uv = 0.0;
							}

							// UPDATE
							double old_est = m_ic.m_graph.Estimate(pred,infNode);
							double trueProb = m_ic.m_graph.Probability(pred,infNode);
							int eta = m_ic.m_graph.getETA(pred,infNode);
							double new_es = old_est + ((double)(x_uv - old_est) / eta);
							//cout << i << " (" << pred << "->" << infNode <<") " << trueProb << " " << x_uv << " " << old_est << " " << new_es << " " << eta <<endl;
							assert(new_es <= 1.01);
							//cout << x_uv << " ";

							if (new_es > 1.01 || isnan(new_es)) {
								cout << "new_es > 1: " << new_es << " " <<old_est <<" " << x_uv << " "  << eta;
								exit(0);
							}
							m_ic.m_graph.UpdateEstimate(pred,infNode, new_es);
						}
					}
				}
				
				//print_nodes(m_ic.m_graph,0);
				//print_nodes(m_ic.m_graph,1);
				m_ic.m_graph.ResetAttributes();
				
			}
			print_graph(m_ic.m_graph);
			cout << "L1 Error is - " << computeL1() << endl;
		}

		void SidNodeLevel(int iterations) {
			int environment = 1;
			for (int i=0;i<iterations;i++){
				cout << "L1 Error is - " << computeL1() << endl;
				// Do bandit round
				vector<int> seeds = explore();
				int spread = m_ic.diffusion(seeds, environment);
				cout << "Explore seeds " << seeds <<endl;
				cout << "Spread - " << spread <<endl;
				//exit(0);

				// UPDATE STEP

				// for each activated node, compute pr of activation
				set<int> allInfNodes = m_ic.m_graph.getAllInfluencedNodes();
				for (auto infNode: allInfNodes ) {
					int infNodeInd = m_ic.m_graph.getNodeIndex(infNode);
					int timeNode = m_ic.m_graph.Timestep(infNode);
					//cout << m_ic.m_graph.IsInfluenced(infNode) <<" " << timeNode <<endl;
					vector<int> preds = m_ic.m_graph.Predecessors(infNode);

					double probFail = 1;
					double probSuccess = 0;
					int flag = 0;

					// this code basically computes the success probability
					for (auto pred: preds){
						if(m_ic.m_graph.IsInfluenced(pred) && m_ic.m_graph.Timestep(pred)<timeNode && m_ic.m_graph.Timestep(pred)>=0) {
							// (S- U S+) if the parent was active at any time before t, an attempt was made
							m_ic.m_graph.triggerETA(pred,infNode);
						}

						if (m_ic.m_graph.IsInfluenced(pred) && m_ic.m_graph.Timestep(pred)==timeNode-1) {
							// (S+) active parents of infNode at time t-1
							probFail *= (1-m_ic.m_graph.Estimate(pred,infNode));
							flag = 1;
						}
					}

					//if (flag == 1 and probFail!=1) {
					if (flag ==1) {
						// if flag is zero, then it must be seed set, we can put an assert here
						//cout << infNode <<endl << "" << "" <<timeNode << " "<<  preds.size() << "(" << preds << ")" << flag << " " << 1-probFail <<endl;
						//assert(std::find(seeds.begin(), seeds.end(), infNode) == seeds.end());
						if (std::find(seeds.begin(), seeds.end(), infNode) != seeds.end()) {
							if (probFail!=1) {
								cout << "flag=1 founds in seeds: " << infNode <<endl;
								cout<< infNode <<endl << "" << "" <<timeNode << " "<<  preds.size() << "(" << preds << ")" << flag << " " << 1-probFail <<endl;								
								exit(0);
							}
							else {
								cout << "flag=1 founds in seeds: " << infNode <<endl;
								cout << infNode <<endl << "" << "" <<timeNode << " "<<  preds.size() << "(" << preds << ")" << flag << " " << 1-probFail <<endl;								
								exit(0);
								// probibility fail = 1
							}
						}
						
						probSuccess = 1 - probFail;
						//assert (probSuccess <= 1.01);
						if (probSuccess > 1.01) {
							cout << "probSuccess > 1: " <<endl;
						}

						for (auto pred:preds) {
							double x_uv = 0.0;
							double old_est = m_ic.m_graph.Estimate(pred,infNode);
							//cout << m_ic.m_graph.IsInfluenced(pred) << " " << (m_ic.m_graph.Timestep(pred)==timeNode-1) << " | "; 


							if(m_ic.m_graph.IsInfluenced(pred) && m_ic.m_graph.Timestep(pred)<timeNode && m_ic.m_graph.Timestep(pred)>=0) {
								// (S- U S+) if the parent was active at any time before t, an attempt was made
								x_uv = 0;
							}

							if (m_ic.m_graph.IsInfluenced(pred) && m_ic.m_graph.Timestep(pred)==timeNode-1) {
								// (S+) active parents of infNode at time t-1
								// was able to influence
								x_uv = (double)old_est/probSuccess;
								cout << old_est << "/" << probSuccess << "=" <<x_uv <<" | " ;
								//x_uv = 1;
								//assert (x_uv <= 1.01);
								if (x_uv > 1.01 || isnan(x_uv)) {
									cout << "x_uv>1: " << old_est << "/" << probSuccess << "=" <<x_uv <<endl ;
									print_graph(m_ic.m_graph);
									exit(0);
								}
							}

							if(m_ic.m_graph.IsInfluenced(pred) && m_ic.m_graph.Timestep(pred)<timeNode && m_ic.m_graph.Timestep(pred)>=0) {
								// (S- U S+) if the parent was active at any time before t, an attempt was made
								
								//update
								int eta = m_ic.m_graph.getETA(pred,infNode);
								double new_es = old_est + (double)(x_uv - old_est) / eta;
								//assert(new_es <= 1.01);
								//cout << x_uv << " ";

								if (new_es > 1.01 || isnan(new_es)) {
									cout << "new_es > 1: " << new_es << " " <<old_est <<" " << x_uv << " "  << eta;
									exit(0);
								}
								m_ic.m_graph.UpdateEstimate(pred,infNode, new_es);
							}
						}
					}
				}

				m_ic.m_graph.ResetAttributes();				

			}
			//print_graph(m_ic.m_graph);

		}

		double computeL1() {
			vector <double> probs = m_ic.m_graph.Probabilities();
			vector <double> estimates = m_ic.m_graph.Estimates();
			double total_diff = 0;
			for (int i =0;i<m_ic.m_graph.NumberOfEdges();i++){
				double diff = sqrt((probs[i] - estimates[i]) * (probs[i] - estimates[i]));
				total_diff += diff;
			}
			return (double)total_diff/m_ic.m_graph.NumberOfEdges();
		}

		void sharanEdgeFeedback(int iterations){
			int environment = 1;
			for (int i=0;i<iterations;i++){
				cout << "L1 Error is - " << computeL1() << endl;
				// Do bandit round
				vector<int> seeds = explore();
				int spread = m_ic.diffusion(seeds, environment);
				//cout << "Spread - " << spread;
				m_ic.m_graph.ResetAttributes();
				
				// update graph
				vector<pair<int,int>> edges = m_ic.m_graph.Edges();
				vector <int> sh_trig = m_ic.m_graph.sharanTriggeredCounts();
				vector <int> sh_act = m_ic.m_graph.sharanActivatedCounts();
				for (int j =0;j<m_ic.m_graph.NumberOfEdges();j++){
					pair<int,int> edge = edges[j];
					int node1 = edge.first;
					int node2 = edge.second;
					if (sh_trig[j]!=0){
						double new_est = (double)sh_act[j]/ sh_trig[j];
						//cout << new_est << endl;
						m_ic.m_graph.UpdateEstimate(node1, node2, new_est);						
					}
				}
			}
			//print_graph(m_ic.m_graph);
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

int sharanEdgeLevelExp()
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
	int iterations = 400;
	 // int environment = 1; // true probablities
	// vector <int> explore_seeds = mab.explore();
	// cout << explore_seeds <<endl;
	 // vector <int> exploit_seeds = mab.exploit(environment);
	 // cout << exploit_seeds <<endl;
	// vector <int> eg_seeds = mab.epsilonGreedy();
	// cout << eg_seeds <<endl;

	 cout << mab.exploit(0) <<endl;
	 mab.runMAB(feedback, iterations);
	 cout << mab.exploit(0) <<endl;

	// double ex_spread = mab.expectedSpread(exploit_seeds, mc_iter, environment);
	// cout << "Expected Spread - " << ex_spread <<endl;

	cout << "\nEnded" << endl;
  return 0;
}


int main()
{
	srand((unsigned)time(0));
	double epsilon = 0.1;
	
	cout << "\nStarted\n" << endl;
	 int m_n = 15229;
	 int m_m = 62752;
	 Xgraph xg = readData("nethept/graph_ic.inf", m_n, m_m);

	//int m_n = 500;
	//int m_m = 2509;
	//Xgraph xg = readData("nethept/synth_500.txt", m_n, m_m);

	// play area
	print_graph(xg);
	//cout << xg.Probabilities();
	//vector <int> seeds = run(xg, 50, 0.1, "IC");
	//cout <<seeds;
	

	//vector<int> simar =  SWIFF_KNIFE::range(0,9);
	//int a = ic.diffusion(seeds, 1);
	//	spreads.push_back(a);
	//}

	// COMPUTE EXPECTED SPREAD
	 // int mc_iter = 100;
	 // int budget = 10;
	 // int environment = 1;
	 // double expSp = expectedSpreadGraph(xg, mc_iter, budget, environment, epsilon);//spreads);
	 // cout << "Expected spead of graph - "<< expSp;
	 // exit(0);

	// class MAB functions
	IC ic(xg);
	MAB mab(10, ic);
	int feedback = 1;
	//int iterations = 5000;
	int iterations = 10000;

	 // int environment = 1; // true probablities
	// vector <int> explore_seeds = mab.explore();
	// cout << explore_seeds <<endl;
	 // vector <int> exploit_seeds = mab.exploit(environment);
	 // cout << exploit_seeds <<endl;
	// vector <int> eg_seeds = mab.epsilonGreedy();
	// cout << eg_seeds <<endl;

	 //cout << "Explot " << mab.exploit(0) <<endl;
	 mab.runMAB(feedback, iterations);
	 //cout << "Explot " <<  mab.exploit(0) <<endl;

	// double ex_spread = mab.expectedSpread(exploit_seeds, mc_iter, environment);
	// cout << "Expected Spread - " << ex_spread <<endl;

	cout << "\nEnded" << endl;
  return 0;
}
