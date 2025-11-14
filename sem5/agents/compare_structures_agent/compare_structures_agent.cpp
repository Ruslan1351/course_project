#include "compare_structures_agent.hpp"

#include "keynodes/compare_structures_keynodes.hpp"

namespace dm
{

ScAddr CompareStructuresAgent::GetActionClass() const
{
  return CompareStructuresKeynodes::action_compare_structures;
}

bool CompareStructuresAgent::AreVertexesEqual(ScAddr vertex1, ScAddr vertex2)
{
  if (vertex1 == vertex2) 
    return true;
  ScType vertex1Type = m_context.GetElementType(vertex1);
  ScType vertex2Type = m_context.GetElementType(vertex2);
  if (vertex1Type != vertex2Type) 
    return false;
  std::string const & vertex1SystemIdtf = m_context.GetElementSystemIdentifier(vertex1);
  std::string const & vertex2SystemIdtf = m_context.GetElementSystemIdentifier(vertex2);
  return vertex1SystemIdtf == vertex2SystemIdtf;
}

bool CompareStructuresAgent::AreArcsEqual(ScAddr arc1, ScAddr arc2)
{
  if (arc1 == arc2) 
    return true;
  ScType arc1Type = m_context.GetElementType(arc1);
  ScType arc2Type = m_context.GetElementType(arc2);
  if (arc1Type != arc2Type) 
    return false;
  ScAddr source1 = m_context.GetArcSourceElement(arc1);
  ScAddr target1 = m_context.GetArcTargetElement(arc1);
  ScAddr source2 = m_context.GetArcSourceElement(arc2);
  ScAddr target2 = m_context.GetArcTargetElement(arc2);
  return AreVertexesEqual(source1, source2) && AreVertexesEqual(target1, target2);
}

ScResult CompareStructuresAgent::DoProgram(ScAction & action)
{
  ScAddr const & struct1Addr = action.GetArgument(ScKeynodes::rrel_1);
  ScAddr const & struct2Addr = action.GetArgument(ScKeynodes::rrel_2);
  if (!m_context.IsElement(struct1Addr) or !m_context.IsElement(struct2Addr)) {
    m_logger.Warning("Action does not have one of arguments(or all).");
    return action.FinishWithError();
  }
  
  ScAddr const & structWithMissingElementsAddr = m_context.GenerateNode(ScType::ConstNodeStructure);
  ScAddr const & structWithExtraElementsAddr = m_context.GenerateNode(ScType::ConstNodeStructure);
  
  std::vector <ScAddr> vertexes1, vertexes2;

  ScTemplate templ1;
  templ1.Triple(
    struct1Addr,
    ScType::VarPermPosArc,
    ScType::VarNode >> "_vertex"
  );
  ScTemplateSearchResult templ1_result;
  m_context.SearchByTemplate(templ1, templ1_result);
  for (size_t i = 0; i < templ1_result.Size(); i++) {
    ScTemplateResultItem vertex = templ1_result[i];
    ScAddr const & vertexAddr = vertex["_vertex"];
    vertexes1.push_back(vertexAddr);
  }

  ScTemplate templ2;
  templ2.Triple(
    struct2Addr,
    ScType::VarPermPosArc,
    ScType::VarNode >> "_vertex"
  );
  ScTemplateSearchResult templ2_result;
  m_context.SearchByTemplate(templ1, templ2_result);
  for (size_t i = 0; i < templ2_result.Size(); i++) {
    ScTemplateResultItem vertex = templ2_result[i];
    ScAddr const & vertexAddr = vertex["_vertex"];
    vertexes2.push_back(vertexAddr);
  }

  for (const auto& vertex2 : vertexes2) {
    bool flag = 1;
    for (const auto& vertex1: vertexes1)
      if (AreVertexesEqual(vertex1, vertex2)) {
        flag = 0;
        break;
      }
    if (flag)
      m_context.GenerateConnector(ScType::ConstPermPosArc, structWithMissingElementsAddr, vertex2);
  }

  for (const auto& vertex1 : vertexes1) {
    bool flag = 1;
    for (const auto& vertex2: vertexes2)
      if (AreVertexesEqual(vertex1, vertex2)) {
        flag = 0;
        break;
      }
    if (flag)
      m_context.GenerateConnector(ScType::ConstPermPosArc, structWithExtraElementsAddr, vertex1);
  }

   std::vector <ScAddr> arcs1, arcs2;

  ScTemplate templ3;
  templ3.Triple(
    struct1Addr,
    ScType::VarPermPosArc,
    ScType::VarArc >> "_arc"
  );
  ScTemplateSearchResult templ3_result;
  m_context.SearchByTemplate(templ3, templ3_result);
  for (size_t i = 0; i < templ3_result.Size(); i++) {
    ScTemplateResultItem arc = templ3_result[i];
    ScAddr const & arcAddr = arc["_arc"];
    arcs1.push_back(arcAddr);
  }

  ScTemplate templ4;
  templ4.Triple(
    struct2Addr,
    ScType::VarPermPosArc,
    ScType::VarArc >> "_arc"
  );
  ScTemplateSearchResult templ4_result;
  m_context.SearchByTemplate(templ4, templ4_result);
  for (size_t i = 0; i < templ4_result.Size(); i++) {
    ScTemplateResultItem arc = templ4_result[i];
    ScAddr const & arcAddr = arc["_arc"];
    arcs2.push_back(arcAddr);
  }
  
  for (const auto& arc2 : arcs2) {
    bool flag = 1;
    for (const auto& arc1: arcs1)
      if (AreVertexesEqual(arc1, arc2)) {
        flag = 0;
        break;
      }
    if (flag)
      m_context.GenerateConnector(ScType::ConstPermPosArc, structWithMissingElementsAddr, arc2);
  }

  for (const auto& arc1 : arcs1) {
    bool flag = 1;
    for (const auto& arc2: arcs2)
      if (AreVertexesEqual(arc1, arc2)) {
        flag = 0;
        break;
      }
    if (flag)
      m_context.GenerateConnector(ScType::ConstPermPosArc, structWithExtraElementsAddr, arc1);
  }

  action.FormResult(structWithMissingElementsAddr, structWithExtraElementsAddr);
  return action.FinishSuccessfully();  
}

}  // namespace dm
