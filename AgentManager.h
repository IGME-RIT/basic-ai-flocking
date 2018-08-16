

#ifndef _AGENT_MANAGER_H
#define _AGENT_MANAGER_H


#include "GLIncludes.h"
#include "GameObject.h"

std::vector<GameObject*> agents;
std::vector<glm::vec3> neighbours;

glm::vec3 target;
glm::vec3 groupForward;
glm::vec3 groupCenter;
glm::vec3 desiredVel;
glm::vec3 steeringForce;


float maxV = 0.005f;
float minDist=0.1f;

//Behaviour to move to a target
glm::vec3 Seek(GameObject agent, glm::vec3 t)
{
	desiredVel = glm::normalize(t - agent.Position())*maxV;
	return (desiredVel - agent.Velocity());
}

//Behaviour to avoid a target
glm::vec3 Avoid(GameObject agent, glm::vec3 t) 
{
	desiredVel = glm::normalize((agent.Position()-t));
	return (desiredVel - agent.Velocity());
}a


//Behaviour to keep away from other agents
void Seperate(GameObject agent)
{

	neighbours.clear();
	glm::vec3 avoidForce =glm::vec3();
	for (int i = 0; i < agents.size(); i++)
	{
		if (agents[i]->Position() != agent.Position())
		{
			if (glm::length(agents[i]->Position() - agent.Position()) < minDist)
				neighbours.push_back(agents[i]->Position());
		}
	}

	for each (glm::vec3 n in neighbours)
		avoidForce += Avoid(agent, n);

	if (neighbours.size() > 0)
	{
		//avoidForce /= neighbours.size();
		steeringForce += avoidForce ;

	}
	
}

//Behaviour to move in the same direction as otehr flocking agents
void Align(GameObject agent)
{
	desiredVel = groupForward*maxV;W
	steeringForce += desiredVel - agent.Velocity();
}

//Behaviour to stay close to other agents
void Cohesion(GameObject agent)
{
	steeringForce += Seek(agent, groupCenter)*0.5f;
	
}

//Parent behaviour that utilizises the above methods to simulate flocking behaviour
void Flocking(std::vector<GameObject*> bodies)W
{
	agents = bodies;


	groupForward = glm::vec3();
	groupCenter= glm::vec3();
	for each (GameObject* agent in agents)
	{
		groupForward += agent->forward;
		groupCenter += agent->Position();
	}
	groupForward /= agents.size();
	groupCenter/= agents.size();
	glm::normalize(groupForward);
	
	for each (GameObject* agent in agents)
	{
		steeringForce = glm::vec3();

		steeringForce += Seek(*agent,target);
		Seperate(*agent);
		Cohesion(*agent);
		Align(*agent);
		glm::clamp(steeringForce, glm::vec3(-.001,-.001,0), glm::vec3(.001,.001,0));
		
		agent->totalForce += steeringForce;
	}
}

#endif _AGENT_MANAGER_H