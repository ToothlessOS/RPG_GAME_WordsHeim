#include <iostream>
#include<string>
#include<fstream>
#include<sstream>
#include <vector>
#include <map>
#include<random>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
using namespace std;
void SelectMove();
void SelectMove_Wild();
void Shop();
void LoadGame();
void Save();
enum Location {//地点：定义
	ChrisTown, ChrisTownNorth, ChrisTownEast
};
struct LOC {
	Location location;
	Location getLocation() const {
		return location;
	}
	LOC() :location() {}
	void setLocation(Location loc) {
		location = loc;
	}
};
LOC lplayer,safepoint;
std::string locationName(Location loc);
void ShowLocation(LOC& loc);
Location currentLocation;

class player {
public:
	int HP;
	int HPmax;
	int DEF;
	int ATK;
	int Coin;
	int Lev;
	int Exp;
	int ExpMax, LevelUp_n;
	string position;
	void LevelUp() {
		LevelUp_n = 0;
		while (Exp > ExpMax) {
			Exp = Exp - ExpMax;
			LevelUp_n++;
			ExpMax = 5 + 5 * (Lev + LevelUp_n);
		}
		if (LevelUp_n > 0) {
			cout << "恭喜！您的等级提升了<" << LevelUp_n << ">级！" << endl;
		}
		Lev = Lev + LevelUp_n;
		HPmax = 10 + (Lev - 1) * 2;
		ATK = 3 + (Lev - 1) * 1;
	}
	void InitProperty() {//新的游戏
		Lev = 1;
		ExpMax = 5 + 5 * Lev;
		HPmax = 10 + (Lev - 1) * 2;
		DEF = 0;
		ATK = 2 + (Lev - 1) * 1;
		Coin = 0;
		Exp = 0;
	}
	void SetLevel() {//等级修改器
		cout << "Your Level is:";
		cin >> Lev;
		HPmax = 10+(Lev-1)*2;
		DEF = 0;
		ATK = 3+(Lev-1)*1;
		Coin = 0;
	}
	void setProperty() {//继续游戏，属性由等级赋值
		ATK = 3 + (Lev - 1) * 1;
		HPmax = 10 + (Lev - 1) * 2;
	}
	void ShowCoin() {
		int key;
		cout << "您的余额为" << Coin << endl;
		cout << "输入1以返回" << endl;
		cin >> key;
		while (key != 1) {
			cout << "错误输入" << endl;
			cin.clear();
			cin.ignore(1024, '\n');
			cin >> key;
		}
		system("cls");
		SelectMove();
	}
	void ShowProperty() {
		int key;
		cout << "您的等级为<" << Lev <<">" << endl;
		cout << "HP:" << HP << "/"<<HPmax<<endl;
		cout << "DEF:" << DEF << endl;
		cout << "ATK:" << ATK << endl;
		cout << "Exp:" << Exp << "/" << ExpMax << endl;
		cout << "输入1以返回" << endl;
		cin >> key;
		while (key != 1) {
			cout << "错误输入" << endl;
			cin.clear();
			cin.ignore(1024, '\n');
			cin >> key;
		}
		system("cls");
		SelectMove();
	}
}oplayer;

class monster {
public:
	string NAME;
	int HP;
	int DEF;
	int ATK;
	int HPmax;
	int Exp;
	int Coin;
	friend player;
	void setProperty(string mn,int mh = 0, int md = 0, int ma = 0, int me=0,int mm=0) {
		NAME = mn;
		HP = mh;
		DEF = md;
		ATK = ma;
		HPmax = mh;
		Exp = me;
		Coin = mm;
	}
	void mAttack(monster m) {
		if (oplayer.HP - m.ATK + oplayer.DEF >= oplayer.HP) {
			oplayer.HP = oplayer.HP - 1;
			cout << "并造成了 1 点伤害！" << endl;
		}
		else {
			oplayer.HP = oplayer.HP - m.ATK + oplayer.DEF;
			cout << "并造成了 "<< m.ATK - oplayer.DEF<<" 点伤害！" << endl;
		}
	}
}Slime,Goblin, Elite_Goblin;
void UseProp(monster& m);

void setMonster() {//HP,DEF,ATK,Exp,Coin//
	Slime.setProperty("史莱姆", 4, 0, 1, 2, 1);
	Goblin.setProperty("哥布林",7, 0, 3, 4, 3);
	Elite_Goblin.setProperty("哥布林精英", 12, 1, 4, 10, 6);
}

class Prop_Potion {
public:
	int n;
	int Eff;
	int cost;
	void setProperty(int pn,int peff,int pcost) {
		n = pn;
		Eff = peff;
		cost = pcost;
	}
}chpp,shpp;

void ShowProp() {
	int key;
	cout << "1 粗制生命药水  数量  " << chpp.n << endl;
	cout << "2 制式生命药水  数量  " << shpp.n << endl;
	cout << "输入1以返回" << endl;
	cin >> key;
	while (key != 1) {
		cout << "错误输入" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
		cin >> key;
	}
	system("cls");
	SelectMove();
}

void PPsetProperty() {//n,Eff,Cost//
	chpp.setProperty(0, 4, 5);
	shpp.setProperty(0, 9, 10);
}

void startGame() {
	int key;
	cout << "Welcome to WordsHeim!" << endl;
	random_device rd;
	mt19937 gen(rd());
	oplayer.InitProperty();
	setMonster();
	PPsetProperty();
	Sleep(2000);
	cout << "1:继续游戏\n2:新的游戏\n";
	cin >> key;
	system("cls");
	switch (key) {
	case(1):
		LoadGame();
		oplayer.setProperty();
		oplayer.HP = oplayer.HPmax;
		Sleep(2000);
		break;
	case(2):
		cout << "你的等级为<1>" << endl;
		oplayer.HP = oplayer.HPmax;
		Sleep(2000);
		lplayer.setLocation(ChrisTown);
		Sleep(2000);
		break;
	default:
		cout << "无效输入，请重新选择！\n" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
		startGame();
	}
}

map<Location, vector<Location>> locationConnections = {//地点：关系
	{ChrisTown,{ChrisTownNorth, ChrisTownEast}},
	{ChrisTownNorth,{ChrisTown}},
	{ChrisTownEast,{ChrisTown}},
};
string locationName(Location loc) {//地点：枚举数字转字符串
	switch (loc) {
	case ChrisTown:
		return "克里斯镇";
	case ChrisTownNorth:
		return "克里斯镇北郊";
	case ChrisTownEast:
		return "克里斯镇东郊";
		// ...
	default:
		return "未知地点";
	}
}
void ShowLocation(LOC& loc) {
	Location currentLocation = loc.getLocation();
	std::cout << "当前地点：" << locationName(currentLocation) << std::endl;
}
void SelectMove_Fight(monster& m) {
	int SMF_key = 0;
	cout << "选择行动：" << endl;
	cout << "1:攻击" << endl;
	cout << "2:逃跑" << endl;
	cout << "3:使用道具" << endl;
	cin >> SMF_key;
	system("cls");
	switch (SMF_key) {
	case(1):
		if (m.HP - oplayer.ATK + m.DEF >= m.HP) {
			m.HP = m.HP - 1;
			cout << "你 攻击了 "<<m.NAME<<" 并造成了 1 点伤害！" << endl;
		}
		else {
			m.HP = m.HP - oplayer.ATK + m.DEF;
			cout << "你 攻击了 " << m.NAME << " 造成了 " << oplayer.ATK - m.DEF
				<< " 点伤害!" << endl;
		}break;
	case(2):
		cout << "逃跑成功了！" << endl;
		SelectMove_Wild();
		break;
	case(3):
		UseProp(m);
		break;
	default:
		cout << "无效输入，请重新选择！\n" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
		SelectMove_Fight(m);
	}
}
void UseProp(monster& m) {
	int key;
	cout << "你拥有：" << endl;
	cout << "1 粗制生命药水  数量  " << chpp.n << endl;
	cout << "2 制式生命药水  数量  " << shpp.n << endl;
	cout << "3 返回" << endl;
	cin >> key;
	system("cls");
	switch (key) {
	case(1):
		if (chpp.n != 0) {
			cout << "你使用了粗制生命药水!" << endl;
			if (oplayer.HPmax - oplayer.HP < chpp.Eff) {
				oplayer.HP = oplayer.HPmax;
				chpp.n = chpp.n - 1;
				cout << "你的剩余血量：" << oplayer.HP << endl;
			}
			else {
				oplayer.HP = oplayer.HP + chpp.Eff;
				chpp.n = chpp.n - 1;
				cout << "你的剩余血量：" << oplayer.HP << endl;
			}
			UseProp(m);
		}
		else {
			cout << "你没有啊！" << endl;
			UseProp(m);
		}break;
	case(2):
		if (shpp.n != 0) {
			cout << "你使用了制式生命药水!" << endl;
			if (oplayer.HPmax - oplayer.HP < shpp.Eff) {
				oplayer.HP = oplayer.HPmax;
				shpp.n = shpp.n - 1;
				cout << "你的剩余血量：" << oplayer.HP << endl;
			}
			else {
				oplayer.HP = oplayer.HP + shpp.Eff;
				shpp.n = shpp.n - 1;
				cout << "你的剩余血量：" << oplayer.HP << endl;
			}
			UseProp(m);
		}
		else {
			cout << "你没有啊！" << endl;
			UseProp(m);
		}break;
	case(3):
		SelectMove_Fight(m);
		break;
	default:
		cout << "无效输入，请重新选择！\n" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
		UseProp(m);
	}
}
void ShowProperty_F(monster m) {
	if (oplayer.HP < 0) {
		oplayer.HP = 0;
	}
	cout << "你的剩余血量：" << oplayer.HP << endl;
	if (m.HP < 0) {
		m.HP = 0;
	}
	Sleep(1000);
	cout << m.NAME << " 的剩余血量：" << m.HP << endl;
	Sleep(1000);
}
void Fight(monster& m) {
	cout << "发现了 "<<m.NAME<<" ！战斗开始！" << endl;
	cout << "你的剩余血量：" << oplayer.HP << endl;
	Sleep(1000);
	while (oplayer.HP > 0 && m.HP > 0) {
		SelectMove_Fight(m);
		Sleep(1000);
		ShowProperty_F(m);
		if(m.HP<=0){
			cout << "毫无悬念的战斗！金币+"<<m.Coin<<"!经验值+" << m.Exp << "!" << endl;
			m.HP = m.HPmax;
			oplayer.Coin=oplayer.Coin + m.Coin;
			oplayer.Exp = oplayer.Exp + m.Exp;
			oplayer.LevelUp();
			Sleep(1000);
			SelectMove_Wild();
		}
		cout << m.NAME<<" 攻击了 你！" ;
		m.mAttack(m);
		Sleep(1000);
		ShowProperty_F(m);
	}
	if (oplayer.HP <= 0) {
		m.HP = m.HPmax;
		cout << "犹豫就会败北！果断就会白给！" << endl;
		lplayer.location = safepoint.location;
		Sleep(2000);
		SelectMove();
	}
}
void Move(LOC& mLOC) {
	Location currentLocation = mLOC.getLocation();
	cout << "当前位置：" << locationName(currentLocation) << endl;
	const std::vector<Location>& connectedLocations = locationConnections[currentLocation];
	cout << "可前往的地点：\n";
	for (size_t i = 0; i < connectedLocations.size(); ++i) {
		std::cout << i + 1 << ": 前往 " << locationName(connectedLocations[i]) << std::endl;
	}
	int key;
	cin >> key;
	system("cls");
	if (key >= 1 && key <= static_cast<int>(connectedLocations.size())) {
		mLOC.setLocation(connectedLocations[key - 1]);
		cout << "从地点 " << locationName(currentLocation) << " 移动到地点 " 
			<< locationName(mLOC.getLocation()) << endl;
		if (mLOC.getLocation() != ChrisTown) {//地点：安全点
			SelectMove_Wild();
		}
		else { SelectMove(); }
	}
	else {
		cout << "无效的选择，请输入有效的数字"<< endl;
		cin.clear();
		cin.ignore(1024, '\n');
		Move(mLOC);
	}
}

void FindTreasure() {
	cout << "你找到了宝箱！";
	cout << "Coin + 2 ！\n";
	oplayer.Coin = oplayer.Coin + 2;
	SelectMove_Wild();
}

void Explore(double poa,double pob,double poc) {
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<> dis(0.0, 1.0);
	double randomValue = dis(gen);
	if (randomValue < poa) {
		Fight(Slime);
	}
	else if ((poa< randomValue)&&(randomValue < pob)) {
		Fight(Goblin);
	}
	else if ((pob < randomValue) && (randomValue < poc)) {
		Fight(Elite_Goblin);
	}
	else if (poc < randomValue) {
		FindTreasure();
	}
}

void SelectMove_Wild() {
	int key = 0;
	ShowLocation(lplayer);
	Sleep(1500);
	cout << "你的下一步是！" << endl;
	Sleep(1500);
	cout << "1:探索此区域" << endl;
	cout << "2:移动" << endl;
	cin >> key;
	system("cls");
	switch (key) {
	case(1):
		Explore(0.4,0.7,0.8);
	case(2):
		Move(lplayer);	
	default:
		cout << "无效输入，请重新选择！" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
		SelectMove_Wild();
	}
}
void PropShop() {
	int PropShop_key, n;
	cout << "请选择您要购买的商品！" << endl;
	cout << "1：粗制生命药水 "<<chpp.cost<<"coin / 1\n"
		<<"2：制式生命药水 "<<shpp.cost<<"coin / 1\n"
		<<"3:返回" << endl;
	cin >> PropShop_key;
	system("cls");
	switch (PropShop_key) {
	case(1):
		cout << "请输入购买数量" << endl;
		cin >> n;
		system("cls");
		if (n <= 0) { PropShop(); }
		if (oplayer.Coin < n * chpp.cost) {
			cout << "余额不足！！" << endl;
			PropShop();
		}
		else {
			oplayer.Coin = oplayer.Coin - n * chpp.cost;
			chpp.n = chpp.n + n;
			cout << "您已购买 " << n << " 瓶粗制生命药水！\n" <<
				"您的余额为" << oplayer.Coin << " 谢谢光临!" << endl;
			PropShop();
		}break;
	case(2):
		cout << "请输入购买数量" << endl;
		cin >> n;
		system("cls");
		if (n <= 0) { PropShop(); }
		if (oplayer.Coin < n * shpp.cost) {
			cout << "余额不足！！" << endl;
			PropShop();
		}
		else {
			oplayer.Coin = oplayer.Coin - n * shpp.cost;
			shpp.n = shpp.n + n;
			cout << "您已购买 " << n << " 瓶制式生命药水！\n" <<
				"您的余额为" << oplayer.Coin << " 谢谢光临!" << endl;
			PropShop();
		}break;
	case(3):
		Shop();
	default:
		cout << "无效输入，请重新选择！\n" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
		PropShop();
	}
}
void Shop() {
	int Shop_key_1;
	cout << "你来到了商业街！" << endl;
	cout << "1:道具店\n2:武器铺（未开放）\n3:防具铺（未开放）\n4:返回" << endl;
	cin >> Shop_key_1;
	system("cls");
	switch (Shop_key_1) {
	case(1):
		PropShop();
		break;
	case(4):
		SelectMove();
	default:
		cout << "无效输入，请重新选择！\n" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
		Shop();
	}
}
void SelectMove() {
	int key = 0;
	ShowLocation(lplayer);
	safepoint.setLocation(safepoint.getLocation());
	cout << "选择下一步行动吧~" << endl;
	Sleep(1500);
	cout << "1:移动" << endl;
	cout << "2:商业街" << endl;
	cout << "3:在旅店休息" << endl;
	cout << "4:属性" << endl;
	cout << "5:背包" << endl;
	cout << "6:钱包" << endl;
	cout << "7:保存游戏" << endl;
	cout << "8:退出游戏" << endl;
	cin >> key;
	system("cls");
	switch (key) {
	case(1):
		Move(lplayer);
	case(2):
		Shop();
	case(3):
		oplayer.HP = oplayer.HPmax;
		cout << "睡饱后你的内心充满了斗志。" << endl;
		Sleep(4000);
		system("cls");
		SelectMove();
	case(4):
		oplayer.ShowProperty();
	case(5):
		ShowProp();
	case(6):
		oplayer.ShowCoin();
	case(7):
		Save();
		Sleep(2000);
		SelectMove();
	case(8):
		exit(0);
		break;
	default:
		cout << "无效输入，请重新选择！\n" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
		SelectMove();
	}
}

struct SaveData {//等级、经验、金币、安全点、道具1、道具2....
	int Lev;
	int Exp;
	int Coin;
	int Safepoint;
	int n_p0001;
	int n_p0002;
	string serialize() const {
		return to_string(Lev) + "," + to_string(Exp) + "," + to_string(Coin) + "," 
			+to_string(n_p0001) + "," + to_string(n_p0002) +","+to_string(Safepoint);
	}
	bool deserialize(const string& data) {
		istringstream iss(data);
		std::vector<int> values;
		std::string token;
		while (std::getline(iss, token, ',')) {
			try {
				values.push_back(std::stoi(token));
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "反序列化失败，无法转换数据: " << token << std::endl;
				return false;
			}
			catch (const std::out_of_range& e) {
				std::cerr << "反序列化失败，整数超出范围: " << token << std::endl;
				return false;
			}
		}
		values.resize(1000);//新增内容时增加n
		values[5] = 0;		//为新内容设置默认值（后兼容）
		oplayer.Lev = values[0];
		oplayer.Exp = values[1];
		oplayer.Coin = values[2];
		n_p0001 = values[3];
		chpp.n = n_p0001;
		n_p0002 = values[4];
		shpp.n = n_p0002;
		safepoint.location = static_cast<Location>(values[5]);
		return true;
	}
};

void saveGame(const SaveData& saveData, const string& filename) {
	ofstream file(filename);
	if (!file.is_open()) {
		cerr << "保存不成功。\n";
		return;
	}
	else { cout << "保存成功！" << endl; }
	file << saveData.serialize();
	file.close();
}
bool loadGame(SaveData& saveData, const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "打开加载文件失败。\n";
		return false;
	}
	string line;
	getline(file, line);
	file.close();
	return saveData.deserialize(line);
}

void Save() {
	SaveData save01 = { oplayer.Lev,oplayer.Exp,oplayer.Coin,safepoint.location,chpp.n,shpp.n };
	saveGame(save01, "save01.txt");
}
void LoadGame() {
	SaveData loadedSave;
	if (loadGame(loadedSave, "save01.txt")) {
		cout << "游戏加载成功！\n";
	}
	else {
		cout << "游戏加载失败！\n";
	}
}
int main() {
	startGame();
	SelectMove();
	return 0;
}
