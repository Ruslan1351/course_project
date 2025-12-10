#include "sc_agent_of_condensation_calculation.hpp"
#include "keynodes/condensation_calc_keynodes.hpp"

#include <sc-memory/sc_memory_headers.hpp>

namespace dm
{
ScAddr ScAgentOfCondensationCalculation::GetActionClass() const
{
  return CondensationCalculationKeynodes::action_of_condensation_calculation;
}

ScResult ScAgentOfCondensationCalculation::DoProgram(ScAction & action)
{
  m_logger.Info("Agent Of Condensation Calculation started");

	auto const & [arg1Addr] = action.GetArguments<1>();

  if (!m_context.IsElement(arg1Addr))
  {
    m_logger.Error("Action does not have argument 1.");
    return action.FinishWithError();
  }

    // Итератор для получения всех вершин, связанных с arg1Addr
    ScIterator3Ptr it3 = m_context.CreateIterator3(
        arg1Addr,
        ScType::ConstPermPosArc,
        ScType::ConstNode
    );

    // graph - это список пар: (адрес_вершины, список_адресов_смежных_вершин)
    std::vector<std::pair<ScAddr, std::vector<ScAddr>>> graph;

    while (it3->Next()) {
        ScAddr vertexAddr = it3->Get(2); // Получаем узел, который является элементом множества
        ScAddrToVector(vertexAddr, graph);
    }

  // ЗАМЕНА: scc_map изменен с std::map<ScAddr, int> на std::vector<std::pair<ScAddr, int>>.
  // Этот вектор содержит пары (ScAddr, ID_КСС)
  std::vector<std::pair<ScAddr, int>> scc_map;
  std::vector<std::vector<int>> condensation_adj;

  // Сигнатура функции find_condensation_graph обновлена для приема std::vector<std::pair<ScAddr, int>>
  int num_sccs = find_condensation_graph(graph, scc_map, condensation_adj);

  ScStructure result = m_context.GenerateStructure();

  // Создаем узлы для каждой КСС в графе конденсации
  std::vector<ScAddr> nodeAddrs;
  for (int i = 0; i < num_sccs; i += 1) {
    ScAddr nodeAddr = m_context.GenerateNode(ScType::ConstNode);
    nodeAddrs.push_back(nodeAddr);
    result << nodeAddr;
  }

  // Создаем ребра для графа конденсации.
for (int u_scc = 0; u_scc < condensation_adj.size(); ++u_scc) {
    // condensation_adj[u_scc] содержит список индексов смежных КСС (v_scc)
    for (int v_scc : condensation_adj[u_scc]) {
        ScAddr edgeAddr = m_context.GenerateConnector(
                     ScType::ConstCommonArc, nodeAddrs[u_scc], nodeAddrs[v_scc]);
        result << edgeAddr;
    }
}

  action.FormResult(result);

  m_logger.Info("Finish Agent Condensation Calculation");

  return action.FinishSuccessfully();
}

void ScAgentOfCondensationCalculation::ScAddrToVector(const ScAddr &addr,
         std::vector<std::pair<ScAddr,
         std::vector<ScAddr>>> &graph) {
    std::vector<ScAddr> edges;

    // Итератор для получения всех исходящих ребер из 'addr'
    ScIterator3Ptr it3 = m_context.CreateIterator3(
        addr,
        ScType::ConstCommonArc,
        ScType::ConstNode
    );

    while (it3->Next()) {
        ScAddr vertexAddr = it3->Get(2);
        edges.push_back(vertexAddr);
    }

    graph.push_back(std::pair<ScAddr, std::vector<ScAddr>>(addr, edges));
}

void ScAgentOfCondensationCalculation::dfs1(int u, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited, std::stack<int>& st) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs1(v, adj, visited, st);
        }
    }
    st.push(u);
}

void ScAgentOfCondensationCalculation::dfs2(int u, const std::vector<std::vector<int>>& rev_adj, std::vector<bool>& visited, std::vector<int>& component) {
    visited[u] = true;
    component.push_back(u);
    for (int v : rev_adj[u]) {
        if (!visited[v]) {
            dfs2(v, rev_adj, visited, component);
        }
    }
}

int ScAgentOfCondensationCalculation::find_condensation_graph(
    const std::vector<std::pair<ScAddr, std::vector<ScAddr>>>& graph,
    std::vector<std::pair<ScAddr, int>>& scc_map, // СИГНАТУРА ИЗМЕНЕНА: std::map -> std::vector<std::pair<ScAddr, int>>
    std::vector<std::vector<int>>& condensation_adj
) {
    // 1. Преобразование ScAddr в целочисленные ID (0..N-1)

    // std::map<ScAddr, int> addr_to_id была заменена на логику линейного поиска.
    // Индекс в векторе id_to_addr служит целочисленным ID.
    std::vector<ScAddr> id_to_addr;

    // Лямбда для получения/создания ID, использующая линейный поиск по вектору
    auto get_or_create_id = [&](const ScAddr& addr) -> int {
        // Линейный поиск ScAddr в id_to_addr (замена map.find)
        for (size_t i = 0; i < id_to_addr.size(); ++i) {
            if (id_to_addr[i] == addr) {
                return static_cast<int>(i); // Улучшение: Использование static_cast
            }
        }
        // Не найдено, создаем новый ID
        int new_id = static_cast<int>(id_to_addr.size()); // Улучшение: Использование static_cast
        id_to_addr.push_back(addr);
        return new_id;
    };

    // Предварительно просматриваем все вершины, чтобы определить размер графа N
    for (const auto& entry : graph) {
        get_or_create_id(entry.first); // Вершина u
        for (const ScAddr& v_addr : entry.second) {
            get_or_create_id(v_addr); // Вершина v
        }
    }

    int n = id_to_addr.size();
    if (n == 0) return 0; // Пустой граф

    // 2. Создание стандартного списка смежности (adj)
    std::vector<std::vector<int>> adj(n);
    for (const auto& entry : graph) {
        // Мы уверены, что entry.first есть в id_to_addr, поэтому ищем его ID
        int u_id = get_or_create_id(entry.first);

        for (const ScAddr& v_addr : entry.second) {
            // v_addr также должен быть в id_to_addr
            int v_id = get_or_create_id(v_addr);
            adj[u_id].push_back(v_id);
        }
    }

    // --- 3. Выполнение алгоритма Косараджу для нахождения КСС ---

    std::stack<int> st;
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) dfs1(i, adj, visited, st);
    }

    std::vector<std::vector<int>> rev_adj(n);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            rev_adj[v].push_back(u);
        }
    }

    std::fill(visited.begin(), visited.end(), false);
    std::vector<int> scc_id(n, -1);
    int current_scc_id = 0;

    // Очищаем выходной вектор перед заполнением
    scc_map.clear();

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        if (!visited[u]) {
            std::vector<int> component;
            dfs2(u, rev_adj, visited, component);

            for (int node_id : component) {
                scc_id[node_id] = current_scc_id;
                // Заполняем итоговый вектор пар ScAddr -> ID_КСС (замена map[key] = value)
                scc_map.push_back({id_to_addr[node_id], current_scc_id});
            }
            current_scc_id++;
        }
    }

    // --- 4. Построение графа конденсации ---

    int num_sccs = current_scc_id;
    condensation_adj.assign(num_sccs, std::vector<int>());

    // ЗАМЕНА: std::set<std::pair<int, int>> edges заменен на двумерный булев вектор
    // edge_exists, чтобы отслеживать уникальность ребер между КСС.
    std::vector<std::vector<bool>> edge_exists(num_sccs, std::vector<bool>(num_sccs, false));

    for (int u_id = 0; u_id < n; ++u_id) {
        int u_scc = scc_id[u_id];
        for (int v_id : adj[u_id]) {
            int v_scc = scc_id[v_id];

            if (u_scc != v_scc) {
                // Проверяем, существует ли ребро уже, используя булев вектор
                if (!edge_exists[u_scc][v_scc]) {
                    condensation_adj[u_scc].push_back(v_scc);
                    edge_exists[u_scc][v_scc] = true; // Отмечаем ребро как добавленное
                }
            }
        }
    }

    return num_sccs;
}
}