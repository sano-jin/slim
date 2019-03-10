#include "mckay.hpp"
#include "trie.hpp"

#include <iostream>
vertex_list::iterator firstNonTrivialCell(vertex_list *pList) {
  auto beginSentinel = std::begin(*pList);
  auto endSentinel = beginSentinel;

  do {
    endSentinel = getNextSentinel(beginSentinel);

    if (std::next(beginSentinel, 2) != endSentinel) {
      return beginSentinel;
    }
    beginSentinel = endSentinel;
  } while (endSentinel != std::end(*pList));

  return std::end(*pList);
}

Order compareDiscretePropagationListOfInheritedVerticesWithAdjacentLabelsInner(
    InheritedVertex *iVertexA, InheritedVertex *iVertexB) {
  if (iVertexA == nullptr && iVertexB == nullptr) {
    return EQ;
  } else if (iVertexA == nullptr && iVertexB != nullptr) {
    CHECKER("CLASS_SENTINEL is invalid\n");
    exit(EXIT_FAILURE);
  } else if (iVertexA != nullptr && iVertexB == nullptr) {
    CHECKER("CLASS_SENTINEL is invalid\n");
    exit(EXIT_FAILURE);
  } else if (iVertexA->type < iVertexB->type) {
    return LT;
  } else if (iVertexA->type > iVertexB->type) {
    return GT;
  } else if (strcmp(iVertexA->name, iVertexB->name) < 0) {
    return LT;
  } else if (strcmp(iVertexA->name, iVertexB->name) > 0) {
    return GT;
  } else if (iVertexA->conventionalPropagationMemo->size() <
             iVertexB->conventionalPropagationMemo->size()) {
    return LT;
  } else if (iVertexA->conventionalPropagationMemo->size() >
             iVertexB->conventionalPropagationMemo->size()) {
    return GT;
  } else {
    int degree = iVertexA->conventionalPropagationMemo->size();
    int i;
    auto &iStackA = *iVertexA->conventionalPropagationMemo;
    auto &iStackB = *iVertexB->conventionalPropagationMemo;

    for (i = 0; i < degree; i++) {
      if (iStackA[i] < iStackB[i]) {
        return LT;
      } else if (iStackA[i] > iStackB[i]) {
        return GT;
      }
    }

    return EQ;
  }
}

Order compareDiscretePropagationListOfInheritedVerticesWithAdjacentLabelsInnerCaster(
    void *iVertexA, void *iVertexB) {
  return compareDiscretePropagationListOfInheritedVerticesWithAdjacentLabelsInner(
      (InheritedVertex *)iVertexA, (InheritedVertex *)iVertexB);
}

void freePreserveDiscreteProapgationList(vertex_list *pdpList) {
  for (auto &v : *pdpList) {
    if (slim::element::holds_alternative<InheritedVertex>(v)) {
      delete (
          slim::element::get<InheritedVertex>(v).conventionalPropagationMemo);
    }
  }

  delete pdpList;
}

bool insertDiscretePropagationListOfInheritedVerticesWithAdjacentLabelToTable(
    discrete_propagation_lists
        *discretePropagationListsOfInheritedVerticesWithAdjacentLabels,
    vertex_list *dpList, ConvertedGraph *cAfterGraph,
    int gapOfGlobalRootMemID) {
  bool isExisting = true;
  return isExisting;
  // putLabelsToAdjacentVertices(dpList, cAfterGraph, gapOfGlobalRootMemID);
  // vertex_list *preserveDPList = new vertex_list();
  // for (auto &v : *dpList)
  //   preserveDPList->push_back(v);
  // for (auto &v : *dpList)
  //   initializeInheritedVertexAdjacentLabels(
  //       &slim::element::get<InheritedVertex>(v));

  // auto &key = *preserveDPList;
  // auto seniorDPList =
  //     discretePropagationListsOfInheritedVerticesWithAdjacentLabels->find(key);

  // if (seniorDPList ==
  //     std::end(
  //         *discretePropagationListsOfInheritedVerticesWithAdjacentLabels)) {

  //   discretePropagationListsOfInheritedVerticesWithAdjacentLabels->insert(
  //       std::make_pair(key, preserveDPList));
  //   isExisting = FALSE;
  //   return isExisting;
  // } else {
  //   auto iteratorCell = std::begin(*preserveDPList);
  //   auto iteratorCellSenior = std::begin(*seniorDPList->second);

  //   while (iteratorCell != std::end(*preserveDPList)) {
  //     if (*iteratorCell != CLASS_SENTINEL) {
  //       auto &iVertex = slim::element::get<InheritedVertex>(*iteratorCell);
  //       auto &iVertexSenior =
  //           slim::element::get<InheritedVertex>(*iteratorCellSenior);

  //       unionDisjointSetForest(iVertex.equivalenceClassOfIsomorphism,
  //                              iVertexSenior.equivalenceClassOfIsomorphism);
  //     }

  //     iteratorCell = std::next(iteratorCell, 1);
  //     iteratorCellSenior = std::next(iteratorCellSenior, 1);
  //   }

  //   freePreserveDiscreteProapgationList(preserveDPList);

  //   isExisting = TRUE;
  //   return isExisting;
  // }
}

void discretePropagationListDump(vertex_list *dpList) {
  std::cout << *dpList;
  fprintf(stdout, "\n");
  fprintf(stdout, "\n");

  return;
}

Bool isNewSplit(vertex_list::iterator sentinelCell,
                vertex_list::iterator splitCell) {
  for (auto iteratorCell = std::next(sentinelCell, 1);
       iteratorCell != splitCell; iteratorCell = std::next(iteratorCell, 1)) {
    auto &splitIVertex = slim::element::get<InheritedVertex>(*splitCell);
    auto &iteratorIVertex = slim::element::get<InheritedVertex>(*iteratorCell);

    if (isInSameDisjointSetForest(
            splitIVertex.equivalenceClassOfIsomorphism,
            iteratorIVertex.equivalenceClassOfIsomorphism)) {
      return FALSE;
    }
  }

  return TRUE;
}

bool listMcKayInner(
    propagation_list &propagationListOfInheritedVertices,
    ConvertedGraph *cAfterGraph, int gapOfGlobalRootMemID,
    discrete_propagation_lists
        *discretePropagationListsOfInheritedVerticesWithAdjacentLabels) {
  bool isUsefulBranch = true;
  auto stabilizer = propagation_list(propagationListOfInheritedVertices);
  printf("%s:%d\n", __FUNCTION__, __LINE__);
  while (refineConventionalPropagationListByPropagation(stabilizer, cAfterGraph,
                                                        gapOfGlobalRootMemID)) {
  }
  // printf("%s:%d\n", __FUNCTION__, __LINE__);
  // /*
  // CHECKER("###### after stable refinement ######\n");
  // std::cout << *stabilizer << std::endl;
  // //*/

  // auto beginSentinel = firstNonTrivialCell(stabilizer);

  // if (beginSentinel == std::end(*stabilizer)) {
  //   printf("%s:%d\n", __FUNCTION__, __LINE__);
  //   isUsefulBranch =
  //       !insertDiscretePropagationListOfInheritedVerticesWithAdjacentLabelToTable(
  //           discretePropagationListsOfInheritedVerticesWithAdjacentLabels,
  //           stabilizer, cAfterGraph, gapOfGlobalRootMemID);
  //   printf("%s:%d\n", __FUNCTION__, __LINE__);
  // } else {
  //   printf("%s:%d\n", __FUNCTION__, __LINE__);
  //   Bool isFirstLoop = TRUE;

  //   auto endSentinel = getNextSentinel(beginSentinel);
  //   auto sentinelCell =
  //       stabilizer->insert(std::next(beginSentinel, 1), CLASS_SENTINEL);

  //   for (auto iteratorCell = sentinelCell;
  //        std::next(iteratorCell, 1) != endSentinel;
  //        iteratorCell = std::next(iteratorCell, 1)) {
  //     auto splitCell = std::next(iteratorCell, 1);

  //     if (isNewSplit(sentinelCell, splitCell)) {
  //       printf("%s:%d\n", __FUNCTION__, __LINE__);
  //       stabilizer->splice(std::next(beginSentinel, 1), *stabilizer, splitCell);

  //       Bool isUsefulChild = listMcKayInner(
  //           stabilizer, cAfterGraph, gapOfGlobalRootMemID,
  //           discretePropagationListsOfInheritedVerticesWithAdjacentLabels);
  //       printf("%s:%d\n", __FUNCTION__, __LINE__);
  //       stabilizer->splice(std::next(iteratorCell, 1), *stabilizer, splitCell);
  //       printf("%s:%d\n", __FUNCTION__, __LINE__);
  //       if (isFirstLoop) {
  //         isFirstLoop = FALSE;
  //         if (!isUsefulChild) {
  //           isUsefulBranch = FALSE;
  //           break;
  //         } else {
  //           isUsefulBranch = TRUE;
  //         }
  //       }
  //     }
  //   }
  // }

  // delete (stabilizer);

  return isUsefulBranch;
}

propagation_list listMcKay(propagation_list &propagationList,
                           ConvertedGraph *cAfterGraph,
                           int gapOfGlobalRootMemID) {
  propagation_list canonicalDiscreteRefinement;
  if (propagationList.empty()) {
    canonicalDiscreteRefinement = propagation_list(propagationList);
    return canonicalDiscreteRefinement;
  } else {
    auto discretePropagationListsOfInheritedVerticesWithAdjacentLabels =
        new discrete_propagation_lists();

    std::cout << "+++++ start classify +++++" << std::endl;
    classifyWithAttribute(propagationList, cAfterGraph, gapOfGlobalRootMemID);
    std::cout << "###### after attribute classifying ######" << std::endl;
    std::cout << propagationList << std::endl;
    listMcKayInner(
        propagationList, cAfterGraph, gapOfGlobalRootMemID,
        discretePropagationListsOfInheritedVerticesWithAdjacentLabels);

    //   vertex_list *canonicalDiscreteRefinement = new vertex_list();
    //   for (auto &v :
    //   *discretePropagationListsOfInheritedVerticesWithAdjacentLabels->begin()->second)
    //     canonicalDiscreteRefinement->push_back(v);

    //   std::cout << "########### candidates of canonical discrete
    //   refinement###########" << std::endl; std::cout <<
    //   *discretePropagationListsOfInheritedVerticesWithAdjacentLabels <<
    //   std::endl;;

    //   for (auto &v :
    //   *discretePropagationListsOfInheritedVerticesWithAdjacentLabels)
    //     freePreserveDiscreteProapgationList(v.second);
    //   delete discretePropagationListsOfInheritedVerticesWithAdjacentLabels;

    //   return canonicalDiscreteRefinement;
  }
  // printf("%s:%d\n", __FUNCTION__, __LINE__);
  return canonicalDiscreteRefinement;
}

Bool checkIsomorphismValidity(unbound_vector<vertex_list *> *slimKeyCollection,
                              key_collection *McKayKeyCollection,
                              vertex_list *canonicalDiscreteRefinement,
                              long stateID) {
  Bool isValid = TRUE;

  if (stateID != 0) {
    auto &key = *canonicalDiscreteRefinement;
    auto it = McKayKeyCollection->find(key);
    if (it != std::end(*McKayKeyCollection)) {
      CollectionInt seniorID = it->second - 1;
      if (stateID != seniorID) {
        fprintf(stdout, "stateID is wrong.\n");
        fprintf(stdout, "juniorStateID is %ld\n", stateID);
        fprintf(stdout, "seniorStateID is %ld\n", seniorID);
        isValid = FALSE;
        return isValid;
      }
    } else {
      McKayKeyCollection->insert(std::make_pair(key, (stateID + 1)));
    }

    vertex_list *seniorDiscreteRefinement = slimKeyCollection->read(stateID);
    if (seniorDiscreteRefinement != NULL) {
      if (*canonicalDiscreteRefinement != *seniorDiscreteRefinement) {
        printf("adjacency list is wrong.\n");
        isValid = FALSE;
        return isValid;
      } else {
        freePreserveDiscreteProapgationList(canonicalDiscreteRefinement);
      }
    } else {
      slimKeyCollection->write(stateID, canonicalDiscreteRefinement);
    }
  }

  return isValid;
}

propagation_list trieMcKay(Trie *trie, DiffInfo *diffInfo,
                           Graphinfo *cAfterGraph, Graphinfo *cBeforeGraph) {
  int gapOfGlobalRootMemID =
      cBeforeGraph->globalRootMemID - cAfterGraph->globalRootMemID;
  int stepOfPropagation;
  Bool verticesAreCompletelySorted =
      trie->propagate(diffInfo, cAfterGraph, cBeforeGraph, gapOfGlobalRootMemID,
                      &stepOfPropagation);
  if (IS_DIFFERENCE_APPLICATION_MODE && verticesAreCompletelySorted && false) {
    return propagation_list();
  } else {
    printf("%s:%d\n", __FUNCTION__, __LINE__);
    for (auto i = cAfterGraph->cv->atoms.begin();
         i != cAfterGraph->cv->atoms.end(); ++i)
      std::cout << *(i->second->correspondingVertexInTrie) << std::endl;
    propagation_list propagationList;
    trie->conventionalPropagationList(trie->body, propagationList);
    std::cout << "###### before list propagate ######" << std::endl;
    std::cout << propagationList << std::endl;

    auto canonicalDiscreteRefinement =
        listMcKay(propagationList, cAfterGraph->cv, gapOfGlobalRootMemID);

    std::cout << "###### after list propagate ######" << std::endl;
    std::cout << canonicalDiscreteRefinement << std::endl;

    return canonicalDiscreteRefinement;
  }
}
