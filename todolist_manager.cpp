#include "todolist.h"


using namespace std;

// 构造函数
TodolistManager::TodolistManager() : nextId(1) {
    loadFromFile();
}

// 安全输入模板函数实现

int safeCin(const string& prompt, int min, int max) {
    int value;
    while (true) {
        if (!prompt.empty()) {
            cout << prompt;
        }

        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入一个有效的值。" << endl;
        }
        else if (value < min || value > max) {
            cout << "请输入 " << min << " 到 " << max << " 之间的值。" << endl;
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}



// 保存数据到文件
void TodolistManager::saveToFile() {
    ofstream file(dataFile);
    
    for (const auto& todo : todos) {
        file << todo.getId() << " "
            << todo.getContent() << " "
            << todo.getType() << " "
            << todo.isCompleted() << endl;
    }

    file.close();
}

// 从文件加载数据
void TodolistManager::loadFromFile() {
    ifstream file(dataFile);
    if (!file) {
        return; // 文件不存在时直接返回
    }

    todos.clear();
    string line;
    int maxId = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ' ')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 4) {
            int id = stoi(tokens[0]);
            string content = tokens[1];
            int type = stoi(tokens[2]);
            bool completed = (tokens[3] == "1");

            Todo todo(id, content, type);
            todo.setCompleted(completed);
            todos.push_back(todo);

            if (id > maxId) {
                maxId = id;
            }
        }
    }

    nextId = maxId + 1;
    file.close();
}

// 显示表头
void TodolistManager::displayHeader() const {
    cout <<"编号  "
        <<setw(30)<<left <<"事项内容类"<<setw(12)<<"类型"<<"完成？" << endl;
    
}



// 安全获取选择
int TodolistManager::getChoice(int min, int max) const {
    return safeCin("请选择操作: ", min, max);
}

// 程序主循环
void TodolistManager::start() {
    while (true) {
        system("cls || clear");
        cout << "=================================" << endl;
        cout << "      四象限时间管理工具" << endl;
        cout << "=================================" << endl;
        cout << "当前待办事项总数: " << getTodoCount() << endl;
        cout << "=================================" << endl;
        cout << "1. 查看所有事项" << endl;
        cout << "2. 添加新事项" << endl;
        cout << "3. 删除事项" << endl;
        cout << "4. 修改事项" << endl;
        cout << "5. 标记完成/未完成" << endl;
        cout << "6. 按四象限查看" << endl;
        cout << "7. 显示统计信息" << endl;
        cout << "0. 退出程序" << endl;
        cout << "=================================" << endl;

        int choice = getChoice(0, 7);

        switch (choice) {
        case 1:
            displayAll();
            break;
        case 2:
            addTodo();
            break;
        case 3:
            deleteTodo();
            break;
        case 4:
            modifyTodo();
            break;
        case 5:
            markComplete();
            break;
        case 6:
            displayQuadrants();
            break;
        case 7:
            displayStatistics();
            break;
        case 0:
            return;
        }

        cout << "\n按回车键继续...";
        cin.ignore();
        cin.get();
    }
}

// 添加事项
void TodolistManager::addTodo() {
    system("cls || clear");
    cout << "========== 添加新事项 ==========" << endl;

    string content;
    cout << "请输入事项内容: ";
    getline(cin, content);

    if (content.empty()) {
        cout << "事项内容不能为空！" << endl;
        return;
    }

    cout << "\n请选择事项类型:" << endl;
    cout << "1. 重要紧急" << endl;
    cout << "2. 重要不紧急" << endl;
    cout << "3. 紧急不重要" << endl;
    cout << "4. 不紧急不重要" << endl;

    int type = getChoice(1, 4);

    Todo newTodo(nextId++, content, type);
    todos.push_back(newTodo);

    cout << "\n 事项添加成功！ID: " << newTodo.getId() << endl;
    saveToFile();
}

// 删除事项
void TodolistManager::deleteTodo() {
    if (todos.empty()) {
        cout << "当前没有待办事项！" << endl;
        return;
    }

    system("cls || clear");
    cout << "========== 删除事项 ==========" << endl;
    displayAll();

    cout << "\n请输入要删除的事项编号 (0取消): ";
    int id = safeCin("", 0, nextId - 1);

    if (id == 0) {
        return;
    }

    auto it = remove_if(todos.begin(), todos.end(),
        [id](const Todo& todo) { return todo.getId() == id; });

    if (it != todos.end()) {
        todos.erase(it, todos.end());
        cout << " 事项删除成功！" << endl;
        saveToFile();
    }
    else {
        cout << "未找到该编号的事项！" << endl;
    }
}

// 修改事项
void TodolistManager::modifyTodo() {
    if (todos.empty()) {
        cout << "当前没有待办事项！" << endl;
        return;
    }

    system("cls || clear");
    cout << "========== 修改事项 ==========" << endl;
    displayAll();

    cout << "\n请输入要修改的事项编号 (0取消): ";
    int id = safeCin("", 0, nextId - 1);

    if (id == 0) {
        return;
    }

    for (auto& todo : todos) {
        if (todo.getId() == id) {
            cout << "\n当前事项内容: " << todo.getContent() << endl;
            cout << "当前事项类型: " << todo.getTypeString() << endl;

            string newContent;
            cout << "\n请输入新的事项内容 (直接回车保持原内容): ";
            getline(cin, newContent);

            if (!newContent.empty()) {
                todo.setContent(newContent);
            }

            cout << "\n是否修改类型? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore();

            if (choice == 'y' || choice == 'Y') {
                cout << "\n请选择新类型:" << endl;
                cout << "1. 重要紧急" << endl;
                cout << "2. 重要不紧急" << endl;
                cout << "3. 紧急不重要" << endl;
                cout << "4. 不紧急不重要" << endl;

                int newType = getChoice(1, 4);
                todo.setType(newType);
            }

            cout << "\n 事项修改成功！" << endl;
            saveToFile();
            return;
        }
    }

    cout << "未找到该编号的事项！" << endl;
}

// 标记完成/未完成
void TodolistManager::markComplete() {
    if (todos.empty()) {
        cout << "当前没有待办事项！" << endl;
        return;
    }

    system("cls || clear");
    cout << "========== 标记完成状态 ==========" << endl;
    displayAll();

    cout << "\n请输入要标记的事项编号 (0取消): ";
    int id = safeCin("", 0, nextId - 1);

    if (id == 0) {
        return;
    }

    for (auto& todo : todos) {
        if (todo.getId() == id) {
            bool current = todo.isCompleted();
            todo.setCompleted(!current);

            cout << "\n 事项已标记为 " << (!current ? "已完成" : "未完成") << "！" << endl;
            saveToFile();
            return;
        }
    }

    cout << "未找到该编号的事项！" << endl;
}

// 显示所有事项
void TodolistManager::displayAll() const {
    system("cls || clear");
    cout << "========== 所有待办事项 ==========" << endl;

    if (todos.empty()) {
        cout << "\n当前没有待办事项！" << endl;
        return;
    }

    displayHeader();

    // 按ID排序显示
    vector<Todo> sortedTodos = todos;
    sort(sortedTodos.begin(), sortedTodos.end(),
        [](const Todo& a, const Todo& b) { return a.getId() < b.getId(); });

    for (const auto& todo : sortedTodos) {
        todo.display();
    }

    

    int completed = count_if(sortedTodos.begin(), sortedTodos.end(),
        [](const Todo& todo) { return todo.isCompleted(); });

    cout << "\n统计: 总计 " << sortedTodos.size() << " 项，已完成 " << completed
        << " 项，未完成 " << (sortedTodos.size() - completed) << " 项" << endl;
}

// 按类型显示
void TodolistManager::displayByType(int type) const {
    cout << "\n========== " << Todo(0, "", type).getTypeString() << " 事项 ==========" << endl;

    vector<Todo> filteredTodos;
    copy_if(todos.begin(), todos.end(), back_inserter(filteredTodos),
        [type](const Todo& todo) { return todo.getType() == type; });

    if (filteredTodos.empty()) {
        cout << "\n该分类下没有待办事项！" << endl;
        return;
    }

    // 按ID排序
    sort(filteredTodos.begin(), filteredTodos.end(),
        [](const Todo& a, const Todo& b) { return a.getId() < b.getId(); });

    displayHeader();
    for (const auto& todo : filteredTodos) {
        todo.display();
    }
    
}

// 显示四象限分类
void TodolistManager::displayQuadrants() const {
    system("cls || clear");
    cout << "========== 四象限分类视图 ==========" << endl;

    for (int type = 1; type <= 4; type++) {
        displayByType(type);
        if (type < 4) {
            cout << "\n";
        }
    }
}

// 显示统计信息
void TodolistManager::displayStatistics() const {
    system("cls || clear");
    cout << "========== 统计信息 ==========" << endl;

    if (todos.empty()) {
        cout << "\n当前没有待办事项！" << endl;
        return;
    }

    vector<int> typeCount(5, 0); // 索引1-4对应四种类型
    vector<int> completedCount(5, 0);

    for (const auto& todo : todos) {
        int type = todo.getType();
        typeCount[type]++;
        if (todo.isCompleted()) {
            completedCount[type]++;
        }
    }

    cout << "\n 按四象限分类统计：" << endl;
    cout << "┌────────────────┬─────────┬─────────┬────────────┐" << endl;
    cout << "│     分类       │  总数   │  已完成 │  完成率    │" << endl;
    cout << "├────────────────┼─────────┼─────────┼────────────┤" << endl;

    string typeNames[] = { "", "重要紧急", "重要不紧急", "紧急不重要", "不紧急不重要" };

    for (int i = 1; i <= 4; i++) {
        double completionRate = typeCount[i] > 0 ?
            (completedCount[i] * 100.0 / typeCount[i]) : 0;

        cout << "│ " << setw(14) << left << typeNames[i]
            << " │ " << setw(7) << typeCount[i]
            << " │ " << setw(7) << completedCount[i]
            << " │ " << setw(9) << fixed << setprecision(1) << completionRate << "% │" << endl;
    }

    cout << "└────────────────┴─────────┴─────────┴────────────┘" << endl;

    int total = getTodoCount();
    int totalCompleted = completedCount[1] + completedCount[2] + completedCount[3] + completedCount[4];
    double totalRate = total > 0 ? (totalCompleted * 100.0 / total) : 0;

    cout << "\n 总体统计：" << endl;
    cout << "  总事项数: " << total << endl;
    cout << "  已完成: " << totalCompleted << endl;
    cout << "  未完成: " << (total - totalCompleted) << endl;
    cout << "  总完成率: " << fixed << setprecision(1) << totalRate << "%" << endl;

    // 显示建议
    cout << "\n 建议：" << endl;
    if (typeCount[2] > 0 && completedCount[2] < typeCount[2]) {
        cout << "  * 注意分配时间处理「重要不紧急」事项，这对长期发展很重要！" << endl;
    }
    if (typeCount[3] > typeCount[1]) {
        cout << "  * 「紧急不重要」事项较多，考虑是否可以委托或简化处理。" << endl;
    }
    if (typeCount[4] > 0) {
        cout << "  * 尽量减少「不紧急不重要」的事项，它们会消耗你的时间和精力。" << endl;
    }
}

// 获取事项总数
int TodolistManager::getTodoCount() const {
    return todos.size();
}