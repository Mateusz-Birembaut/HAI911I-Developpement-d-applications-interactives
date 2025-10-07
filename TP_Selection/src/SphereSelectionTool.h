#ifndef SphereSelectionTool_H
#define SphereSelectionTool_H
#include "Vec3.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

void drawSphere1(float x, float y, float z, float radius, int slices, int stacks) {
	if (stacks < 2) {
		stacks = 2;
	}
	if (stacks > 30) {
		stacks = 30;
	}
	if (slices < 3) {
		slices = 3;
	}
	if (slices > 30) {
		slices = 30;
	}
	// Pas essentiel ...

	int Nb = slices * stacks + 2;
	std::vector<Vec3> points(Nb);

	Vec3 centre(x, y, z);

	float sinP, cosP, sinT, cosT, Phi, Theta;
	points[0] = Vec3(0, 0, 1);
	points[Nb - 1] = Vec3(0, 0, -1);

	for (int i = 1; i <= stacks; i++) {
		Phi = 90 - (float)(i * 180) / (float)(stacks + 1);
		sinP = sinf(Phi * 3.14159265 / 180);
		cosP = cosf(Phi * 3.14159265 / 180);

		for (int j = 1; j <= slices; j++) {
			Theta = (float)(j * 360) / (float)(slices);
			sinT = sinf(Theta * 3.14159265 / 180);
			cosT = cosf(Theta * 3.14159265 / 180);

			points[j + (i - 1) * slices] = Vec3(cosT * cosP, sinT * cosP, sinP);
		}
	}

	int k1, k2;
	glBegin(GL_TRIANGLES);
	for (int i = 1; i <= slices; i++) {
		k1 = i;
		k2 = (i % slices + 1);
		glNormal3f(points[0][0], points[0][1], points[0][2]);
		glVertex3f((centre + radius * points[0])[0], (centre + radius * points[0])[1], (centre + radius * points[0])[2]);
		glNormal3f(points[k1][0], points[k1][1], points[k1][2]);
		glVertex3f((centre + radius * points[k1])[0], (centre + radius * points[k1])[1], (centre + radius * points[k1])[2]);
		glNormal3f(points[k2][0], points[k2][1], points[k2][2]);
		glVertex3f((centre + radius * points[k2])[0], (centre + radius * points[k2])[1], (centre + radius * points[k2])[2]);

		k1 = (stacks - 1) * slices + i;
		k2 = (stacks - 1) * slices + (i % slices + 1);
		glNormal3f(points[k1][0], points[k1][1], points[k1][2]);
		glVertex3f((centre + radius * points[k1])[0], (centre + radius * points[k1])[1], (centre + radius * points[k1])[2]);
		glNormal3f(points[Nb - 1][0], points[Nb - 1][1], points[Nb - 1][2]);
		glVertex3f((centre + radius * points[Nb - 1])[0], (centre + radius * points[Nb - 1])[1], (centre + radius * points[Nb - 1])[2]);
		glNormal3f(points[k2][0], points[k2][1], points[k2][2]);
		glVertex3f((centre + radius * points[k2])[0], (centre + radius * points[k2])[1], (centre + radius * points[k2])[2]);
	}
	glEnd();

	glBegin(GL_QUADS);
	for (int j = 1; j < stacks; j++) {
		for (int i = 1; i <= slices; i++) {
			k1 = i + (j - 1) * slices;
			k2 = (i % slices + 1) + (j - 1) * slices;
			glNormal3f(points[k2][0], points[k2][1], points[k2][2]);
			glVertex3f((centre + radius * points[k2])[0], (centre + radius * points[k2])[1], (centre + radius * points[k2])[2]);
			glNormal3f(points[k1][0], points[k1][1], points[k1][2]);
			glVertex3f((centre + radius * points[k1])[0], (centre + radius * points[k1])[1], (centre + radius * points[k1])[2]);

			k1 = i + (j)*slices;
			k2 = (i % slices + 1) + (j)*slices;
			glNormal3f(points[k1][0], points[k1][1], points[k1][2]);
			glVertex3f((centre + radius * points[k1])[0], (centre + radius * points[k1])[1], (centre + radius * points[k1])[2]);
			glNormal3f(points[k2][0], points[k2][1], points[k2][2]);
			glVertex3f((centre + radius * points[k2])[0], (centre + radius * points[k2])[1], (centre + radius * points[k2])[2]);
		}
	}
	glEnd();
}

struct SphereSelectionTool {
	MeshVertex closestPoint;
	Vec3 center{0.0, 0.0, 0.0};
	float radius;

	bool isAdding;
	bool isActive;

	std::vector<unsigned int> pathToTarget;

	std::map<unsigned int, double> distances;

	SphereSelectionTool() : radius(0.5), center(0.0, 0.0, 0.0), isAdding(false), isActive(false) {}

	void initSphere(const Vec3& pCenter, const float& pRadius, const Mesh& mesh, LaplacianWeights& edgeAndVertexWeights) {
		center = pCenter;
		radius = pRadius;
		closestPoint = mesh.findClosestPoint(center);
		fillDistances(mesh, edgeAndVertexWeights);
        //auto target = mesh.randomVertex();
        //pathToTarget = shortestPath(target, mesh, edgeAndVertexWeights);
	}

	void updateSphere(float pRadius) {
		radius = pRadius;
	}

	bool containsV1(const Vec3& p, const Mesh& mesh) {
		// return (center - p).norm() <= radius;
		auto id = mesh.getIndexOfVertex(p);
		if (id == -1)
			return false;

		if (distances.find(id) != distances.end() && distances.find(id) != distances.end()) {
			return distances[id] <= radius;
		}
		return false;
	}

	bool contains(const Vec3& p, const Mesh& mesh) {
		// return (center - p).norm() <= radius;
		auto id = mesh.getIndexOfVertex(p);
		if (id == -1)
			return false;

		Vec3 normalP = mesh.V[id].n;
		Vec3 normalCenter = closestPoint.n;

		float margin = 0.9f;

		if (distances.find(id) != distances.end() && distances.find(id) != distances.end()) {
			if (distances[id] <= radius && Vec3::dot(normalP, normalCenter) > margin) {
				return true;
			} else {
				return false;
			}
		}

		return false;
	}

	void draw() {

		if (!isActive)
			return;

		GLint polygonMode[2];
		glGetIntegerv(GL_POLYGON_MODE, polygonMode);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDisable(GL_LIGHTING);

		if (isAdding)
			glColor3f(0.0f, 0.0f, 1.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);

		drawSphere1(center[0], center[1], center[2], radius, 10, 10);

		glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);
		glEnable(GL_LIGHTING);
	}

	void drawPath(const Mesh& mesh) {
		if (pathToTarget.empty())
			return;

		glDisable(GL_LIGHTING);
		glLineWidth(3.0f);
		glColor3f(0.0f, 1.0f, 0.0f);

		glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < pathToTarget.size(); ++i) {
			const MeshVertex& vertex = mesh.V[pathToTarget[i]];
			glVertex3f(vertex.p[0], vertex.p[1], vertex.p[2]);
		}
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);
		const MeshVertex& start = mesh.V[pathToTarget[0]];
		drawSphere1(start.p[0], start.p[1], start.p[2], 0.02f, 8, 8);

		glColor3f(1.0f, 0.0f, 1.0f);
		const MeshVertex& end = mesh.V[pathToTarget[pathToTarget.size() - 1]];
		drawSphere1(end.p[0], end.p[1], end.p[2], 0.02f, 8, 8);

		glLineWidth(1.0f);
		glEnable(GL_LIGHTING);
	}

	void fillDistances(const Mesh& mesh, LaplacianWeights& edgeAndVertexWeights) {
		distances.clear();

		std::set<unsigned int> closedList;

		// distance / sommet index
		std::priority_queue<std::pair<double, unsigned int>,
				    std::vector<std::pair<double, unsigned int>>,
				    std::greater<std::pair<double, unsigned int>>>
		    openList;

		unsigned int startIdx = mesh.getIndexOfVertex(closestPoint);

		distances[startIdx] = 0.0;
		openList.push({0.0, startIdx});

		while (!openList.empty()) {
			auto current = openList.top();
			openList.pop();

			double currentDist = current.first;
			unsigned int currentId = current.second;

			if (closedList.find(currentId) != closedList.end()) {
				continue;
			}

			closedList.insert(currentId);

			for (std::map<unsigned int, double>::const_iterator it = edgeAndVertexWeights.get_weight_of_adjacent_edges_it_begin(currentId);
			     it != edgeAndVertexWeights.get_weight_of_adjacent_edges_it_end(currentId); ++it) {

				unsigned int neighborId = it->first;

				if (closedList.find(neighborId) != closedList.end()) {
					continue;
				}

				double newDist = currentDist + (mesh.V[neighborId].p - mesh.V[currentId].p).norm();

				if (distances.find(neighborId) == distances.end() || newDist < distances[neighborId]) {
					distances[neighborId] = newDist;
					openList.push({newDist, neighborId});
				}
			}
		}
	}

	std::vector<unsigned int> shortestPath(const MeshVertex& vertexTarget, const Mesh& mesh, LaplacianWeights& edgeAndVertexWeights) {
		distances.clear();

		std::map<unsigned int, unsigned int> predecessors;

		std::set<unsigned int> closedList;

		// distance / sommet index
		std::priority_queue<std::pair<double, unsigned int>,
				    std::vector<std::pair<double, unsigned int>>,
				    std::greater<std::pair<double, unsigned int>>>
		    openList;

		unsigned int startIdx = mesh.getIndexOfVertex(closestPoint);
		unsigned int targetIdx = mesh.getIndexOfVertex(vertexTarget);

		distances[startIdx] = 0.0;
		openList.push({0.0, startIdx});

		while (!openList.empty()) {
			auto current = openList.top();
			openList.pop();

			double currentDist = current.first;
			unsigned int currentId = current.second;

			if (closedList.find(currentId) != closedList.end()) {
				continue;
			}

			closedList.insert(currentId);

			if (currentId == targetIdx) {
				std::vector<unsigned int> path;
				unsigned int idx = targetIdx;
				while (idx != startIdx) {
					path.push_back(idx);
					idx = predecessors[idx];
				}
				path.push_back(startIdx);
				std::reverse(path.begin(), path.end());
				return path;
			}

			for (std::map<unsigned int, double>::const_iterator it = edgeAndVertexWeights.get_weight_of_adjacent_edges_it_begin(currentId);
			     it != edgeAndVertexWeights.get_weight_of_adjacent_edges_it_end(currentId); ++it) {

				unsigned int neighborId = it->first;
				double edgeWeight = it->second;

				if (closedList.find(neighborId) != closedList.end()) {
					continue;
				}

				double newDist = currentDist + (mesh.V[neighborId].p - mesh.V[currentId].p).norm();

				if (distances.find(neighborId) == distances.end() || newDist < distances[neighborId]) {
					distances[neighborId] = newDist;
					predecessors[neighborId] = currentId;
					openList.push({newDist, neighborId});
				}
			}
		}

		std::cout << "Pas de chemin trouvé" << '\n';
		return std::vector<unsigned int>();
	}
};
#endif