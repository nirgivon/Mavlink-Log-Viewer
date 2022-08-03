#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

#include "ImPlot/implot.h"

#include "Tracy/Tracy.hpp"

#include "imfilebrowser.h"
#include "utils.h"
#include "Data/Param.h"
#include "Data/DataModel.h"

#include <iostream>
#include <istream>
#include <fstream>


using std::string;

bool handleFile = false;


class ExampleLayer : public Walnut::Layer
{

	DataModel dataModel;

	Param p;

	vector<Param*> paramsToGraph;
	Param* timeToGraph;
	//vector<double> timeToGraph;

	void readFile(string filename)
	{
		ZoneScoped;

		std::ifstream logFile(filename);

		string logFileLine;

		size_t lineCount = 0;

		printf("Counting File Size\n");
		//lineCount = count_line(logFile);

		printf("File Size: %zu \n", lineCount);

		long progress = 0;

		int state = 1;

		int cout_baro = 0;

		while (getline(logFile, logFileLine))
		{
			ZoneScopedN("Treat Line");

			progress++;
			float t = progress * 100;
			t = t / lineCount;

			if (t >= state)
			{
				//printf("%f4\n", t);
				state += 1;
			}

			{
				ZoneScopedN("Split Line");
				string delimiter = ",";
				logFileLine.erase(remove(logFileLine.begin(), logFileLine.end(), ' '),logFileLine.end());
				vector<string> tokensList = split(logFileLine, delimiter);

				if (!strcmp(tokensList[0].c_str(), "FMT"))
				{
					//printf("FMT mesg\n");

					string name = tokensList[3];
					int id = std::stoi(tokensList[1]);

					Sensor* sensor = new Sensor(name, id);

					vector<Param*> paramVec;
					sensor->params.push_back(paramVec);

					vector<string> paramNames;
					paramNames.reserve(tokensList.size() - 5);

					for (int i = 5; i < tokensList.size(); i++)
					{
						paramNames.push_back(tokensList[i]);
						
						Param* param = new Param(tokensList[i]);

						sensor->params[0].push_back(param);
					}

					dataModel.sensors.push_back(sensor);


				}
				else if (!strcmp(tokensList[0].c_str(), "UNIT"))
				{
				}
				else if (!strcmp(tokensList[0].c_str(), "FMTU"))
				{
					int id = std::stoi(tokensList[2]);

					for (auto sensor : dataModel.sensors)
					{
						if (id == (sensor->id)) {
							sensor->FMTU = tokensList[3];
							sensor->MULT = tokensList[4];
						}
					}

				}
				else if (!strcmp(tokensList[0].c_str(), "MULT"))
				{
				}
				else if (!strcmp(tokensList[0].c_str(), "PARM"))
				{
				}
				else if (!strcmp(tokensList[0].c_str(), "MSG"))
				{
				}
				else if (!strcmp(tokensList[0].c_str(), "MODE"))
				{
					//printf("MSG mesg\n");
				}
				else if (!strcmp(tokensList[0].c_str(), "CAND"))
				{
					//printf("MSG mesg\n");
				}
				else
				{
					string name = tokensList[0];

					for (auto sensor : dataModel.sensors)
					{
						if (name == sensor->name)
						{
							//printf("somthing");

							double d = std::stod(tokensList[1]);
							double f = (double)(d / 1000000);

							int sensorIndex = 0;

							if (!sensor->FMTU.empty())
							{
								if (sensor->FMTU[1] == '#')
								{
									sensorIndex = std::stoi(tokensList[2]);
									if (sensor->params.size() - 1 < sensorIndex)
									{
										vector<Param*> paramVec;
										for (auto p : sensor->params[0])
										{
											string newName = p->name + std::to_string(sensorIndex);
											Param* np = new Param(newName);
											paramVec.push_back(np);
										}
										sensor->params.push_back(paramVec);
									}
								}
							}

							for (int i = 1; i < tokensList.size(); i++)
							{
								if (i == 1)
								{
									sensor->params[sensorIndex][i - 1]->values.push_back(f);
								}
								else {
									sensor->params[sensorIndex][i - 1]->values.push_back(std::stof(tokensList[i]));
								}
							}
						}
					}

				}

				for (auto token : tokensList)
				{
					//ZoneScopedN("Token loop");
					//printf("%s ", token.c_str());
				}
			}
			//printf("\n");

		}

		printf("Found: %d Sensors\n", dataModel.sensors.size());

		logFile.clear();
		logFile.seekg(0);


		p.values.reserve(cout_baro);

		progress = 0;
		state = 1;

		while (getline(logFile, logFileLine))
		{
			ZoneScopedN("Treat Line");

			progress++;
			float t = progress * 100;
			t = t / lineCount;

			if (t >= state)
			{
				//printf("%f4\n", t);
				state += 1;
			}

			{
				ZoneScopedN("Split Line");
				//auto tokensList = ofSplitString(logFileLine, ",", false, true);
				string delimiter = ",";
				vector<string> tokensList = split(logFileLine, delimiter);

				if (!strcmp(tokensList[0].c_str(), "BARO"))
				{
					//printf("MSG mesg\n");

					p.values.push_back(std::stof(tokensList[3]));
				}

				//if (!strcmp(tokensList[0].c_str(), "FMT"))
				//{
				//	printf("FMT mesg\n");

				//	string name = tokensList[3];
				//	int id = std::stoi(tokensList[1]);

				//	vector<string> paramNames;
				//	paramNames.reserve(tokensList.size() - 5);

				//	for (int i = 5; i < tokensList.size(); i++)
				//	{
				//		paramNames.push_back(tokensList[i]);
				//	}

				//}
				//else if (!strcmp(tokensList[0].c_str(), "PARM"))
				//{
				//	//printf("PARM mesg\n");
				//}
				//else if (!strcmp(tokensList[0].c_str(), "MSG"))
				//{
				//	//printf("MSG mesg\n");
				//}
				//else if (!strcmp(tokensList[0].c_str(), "BARO"))
				//{
				//	//printf("MSG mesg\n");
				//	cout_baro++;
				//}
				//else
				//{
				//	//printf("Untreated command: %s\n", tokensList[0].c_str());
				//}

				//for (auto token : tokensList)
				//{
				//	//ZoneScopedN("Token loop");
				//	//printf("%s ", token.c_str());
				//}
			}
			//printf("\n");

		}

		logFile.close();



		printf("Baro count: %d\n", cout_baro);
	}



public:


	virtual void OnUIRender() override
	{
		ZoneScoped;

		if (handleFile)
		{
			readFile(sFilePath);
			handleFile = false;
		}

		ImGui::Begin("Hello");
		ImGui::Button("Button");

		ImPlot::BeginPlot("plot",ImVec2(-1,-1));

		if (!paramsToGraph.empty()) {
			int plotIndex = 0;
			int axisIndex = ImAxis_Y1;

			ImPlot::SetupAxes("X-Axis 1", "Y-Axis 1");
			//ImPlot::SetupAxis(ImAxis_X1, NULL, ImPlotAxisFlags_Time);
			for (auto p : paramsToGraph)
			{
				if (plotIndex > 0) {
					ImPlot::SetupAxis(axisIndex, p->name.c_str(), ImPlotAxisFlags_None);
				}
				plotIndex++;
				axisIndex++;
			}

			plotIndex = 0;
			axisIndex = ImAxis_Y1;
			for (auto p : paramsToGraph)
			{
				if (plotIndex > 0) {
					ImPlot::SetAxes(0, axisIndex);
				}

				ImPlot::PlotLine(p->name.c_str(), &(timeToGraph->values[0]), &(p->values[0]), p->values.size());
				//ImPlot::PlotLine(p->name.c_str(), &p->values[0], p->values.size());
				plotIndex++;
				axisIndex++;
			}
		}

		ImPlot::EndPlot();
		ImGui::End();

		ImGui::Begin("params");

		static ImGuiTextFilter filter;
		filter.Draw();

		for (auto sensor : dataModel.sensors)
		{

			if (sensor->params[0][0]->values.empty()) continue;

			if (!filter.PassFilter(sensor->name.c_str())) continue;


			if (ImGui::TreeNode(sensor->name.c_str()))
			{
				int sensorIndex = 0;
				bool hasMultSensors = false;

				if (!sensor->FMTU.empty())
				{
					if (sensor->FMTU[1] == '#')
					{
						hasMultSensors = true;
					}
				}

				if (hasMultSensors == true) {
					for (int i = 0; i<sensor->params.size(); i++) {
						auto v = sensor->params[i];

						if (ImGui::TreeNode(std::to_string(i).c_str())) {

							for (auto param : v) {
								ImGui::Selectable(param->name.c_str(), &param->isSelected);

								if (param->isSelected != param->prevIsSelected)
								{
									if (param->isSelected) {
										paramsToGraph.push_back(param);
										timeToGraph = sensor->params[0][0];
									}
									else {

										auto it = find(paramsToGraph.begin(), paramsToGraph.end(), param);

										if (it != paramsToGraph.end()) {
											int index = it - paramsToGraph.begin();
											paramsToGraph.erase(paramsToGraph.begin() + index);

										}
										else {
											printf("ERROR: Param to erase not found");
										}
									}


									param->prevIsSelected = param->isSelected;
								}
							}
							ImGui::TreePop();
						}
					}
				}
				else {
					for (auto param : sensor->params[0])
					{
						ImGui::Selectable(param->name.c_str(), &param->isSelected);

						if (param->isSelected != param->prevIsSelected)
						{
							if (param->isSelected) {
								paramsToGraph.push_back(param);
								timeToGraph = sensor->params[0][0];
							}
							else {

								auto it = find(paramsToGraph.begin(), paramsToGraph.end(), param);

								if (it != paramsToGraph.end()) {
									int index = it - paramsToGraph.begin();
									paramsToGraph.erase(paramsToGraph.begin() + index);

								}
								else {
									printf("ERROR: Param to erase not found");
								}
							}


							param->prevIsSelected = param->isSelected;
						}
					}
				}

				ImGui::TreePop();
			}

		}

		ImGui::End();

		//ImGui::ShowDemoWindow();
		//ImPlot::ShowDemoWindow();

		FrameMark;
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Log File"))
			{
				bool res = openFile();
				switch (res) {
				case true:
					printf("Openning file %s\n", sSelectedFile.c_str());
					handleFile = true;

					//readFile(sFilePath);
					break;

				case false:
					printf("Error with file dialog\n");
					break;
				}
			}
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}






