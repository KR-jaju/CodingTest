
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#define NPOS 2100000000

struct Layer {
public:
	Layer() {
		for (int y = 0; y < 5; ++y) {
			for (int x = 0; x < 5; ++x) {
				this->shape[y][x] = true;
			}
		}
	}
	void	set(int x, int y, bool b) {
		this->shape[y][x] = b;
	}
	bool	get(int x, int y, int rotation) const {
		if (x < 0 || x >= 5 || y < 0 || y >= 5)
			return (true);
		switch (rotation)
		{
		case 0:
			return (this->shape[y][x]);
		case 1:
			return (this->shape[4 - x][y]);
		case 2:
			return (this->shape[4 - y][4 - x]);
		case 3:
			return (this->shape[x][4 - y]);
		}
		return (this->shape[y][x]);
	}
private:
	bool	shape[5][5];
};

class Maze {
public:
	struct DistanceField {
		int	data[5][5];
		DistanceField() {
			for (int y = 0; y < 5; ++y) {
				for (int x = 0; x < 5; ++x) {
					this->data[y][x] = NPOS; // NPOS는 distance가 설정되지 않았음을 의미
				}
			}
		}
		DistanceField	&set(int x, int y, int value) {
			this->data[y][x] = value;
			return (*this);
		}
	};
	struct FillInfo
	{
		int	x;
		int	y;
		int	distance;
		
		FillInfo(int x, int y, int distance): x(x), y(y), distance(distance) {}
		bool	operator<(FillInfo const &other) const {
			return (this->distance < other.distance);
		}
		bool	operator<=(FillInfo const &other) const {
			return (this->distance <= other.distance);
		}
		bool	operator>(FillInfo const &other) const {
			return (this->distance > other.distance);
		}
		bool	operator>=(FillInfo const &other) const {
			return (this->distance >= other.distance);
		}
		bool	operator==(FillInfo const &other) const {
			return (this->distance == other.distance);
		}
		bool	operator!=(FillInfo const &other) const {
			return (this->distance != other.distance);
		}
	};
	Maze() {
		this->distance_field.emplace_back().set(0, 0, 0);
		this->level = 0;
	}
	void	push(Layer const &layer, int rotation) {
		DistanceField const	&prev = this->distance_field.back(); // 가장 위에 있는 레이어
		DistanceField		&curr = this->distance_field.emplace_back();
		std::priority_queue<FillInfo, std::vector<FillInfo>, std::greater<FillInfo>>	queue; // distance, 위치
		for (int y = 0; y < 5; ++y) { 
			for (int x = 0; x < 5; ++x) {
				if (prev.data[y][x] == NPOS || layer.get(x, y, rotation)) // prev가 막혀있거나 현재가 막혀있음
					continue ;
				int distance = prev.data[y][x] + 1;
				queue.push(FillInfo(x, y, distance));
			}
		} // 주입할 distance 모두 넣음
		while (!queue.empty()) {
			int		x = queue.top().x;
			int		y = queue.top().y;
			int		distance = queue.top().distance;
			queue.pop();
			if (layer.get(x, y, rotation) || (curr.data[y][x] != NPOS && distance >= curr.data[y][x])) // 미로가 채워져있거나, 채울 가치가 없음
				continue;
			curr.data[y][x] = distance;
			queue.push(FillInfo(x + 1, y, distance + 1));
			queue.push(FillInfo(x - 1, y, distance + 1));
			queue.push(FillInfo(x, y + 1, distance + 1));
			queue.push(FillInfo(x, y - 1, distance + 1));
		}
		this->level += 1;
	}
	void	pop(void) {
		if (this->level <= 0)
			return ;
		this->distance_field.pop_back();
		this->level -= 1;
	}
	int		distance(void) const {
		return (this->distance_field.back().data[4][4]);
	}
private:
	std::vector<DistanceField>	distance_field;
	int		level;
};

int	main(void) {
	Layer	layer;
	Maze	maze;

	for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 5; ++x) {
			layer.set(x, y, false);
		}
	}
	maze.push(layer, 0);
	std::cout << maze.distance();
}


