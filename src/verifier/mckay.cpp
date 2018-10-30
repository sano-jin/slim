#include "mckay.hpp"
#include "trie.hpp"

#include <iostream>

template <typename List>
void initializeDisjointSetForestsOfPropagationList(List *pList) {
  for (auto iteratorCell = std::begin(*pList); iteratorCell != std::end(*pList);
       iteratorCell = std::next(iteratorCell, 1)) {
    if (*iteratorCell != CLASS_SENTINEL) {
      auto &iVertex = slim::element::get<InheritedVertex>(*iteratorCell);
      initializeDisjointSetForest(iVertex.equivalenceClassOfIsomorphism);
    }
  }

  return;
}

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
  } else if (numStack(iVertexA->conventionalPropagationMemo) <
             numStack(iVertexB->conventionalPropagationMemo)) {
    return LT;
  } else if (numStack(iVertexA->conventionalPropagationMemo) >
             numStack(iVertexB->conventionalPropagationMemo)) {
    return GT;
  } else {
    int degree = numStack(iVertexA->conventionalPropagationMemo);
    int i;
    std::vector<int> *iStackA = iVertexA->conventionalPropagationMemo;
    std::vector<int> *iStackB = iVertexB->conventionalPropagationMemo;

    for (i = 0; i < degree; i++) {
      if (readStack(iStackA, i) < readStack(iStackB, i)) {
        return LT;
      } else if (readStack(iStackA, i) > readStack(iStackB, i)) {
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

void initializeInheritedVertexAdjacentLabels(InheritedVertex *iVertex) {
  if (iVertex == nullptr) {
    return;
  } else {
    iVertex->conventionalPropagationMemo->clear();

    return;
  }
}

void initializeInheritedVertexAdjacentLabelsCaster(void *iVertex) {
  initializeInheritedVertexAdjacentLabels((InheritedVertex *)iVertex);

  return;
}

void freeInheritedVertexOfPreserveDiscretePropagationList(
    InheritedVertex *iVertex) {
  if (iVertex != nullptr) {
    freeStack(iVertex->conventionalPropagationMemo);
    free(iVertex);
  }

  return;
}

void freePreserveDiscreteProapgationList(vertex_list *pdpList) {
  for (auto &v : *pdpList) {
    if (slim::element::holds_alternative<InheritedVertex>(v)) {
      freeStack(
          slim::element::get<InheritedVertex>(v).conventionalPropagationMemo);
    }
  }

  delete pdpList;
}

Bool insertDiscretePropagationListOfInheritedVerticesWithAdjacentLabelToTable(
    RedBlackTree__<KeyContainer__<vertex_list *>, vertex_list *>
        *discretePropagationListsOfInheritedVerticesWithAdjacentLabels,
    vertex_list *dpList, ConvertedGraph *cAfterGraph,
    int gapOfGlobalRootMemID) {
  Bool isExisting;

  putLabelsToAdjacentVertices(dpList, cAfterGraph, gapOfGlobalRootMemID);
  vertex_list *preserveDPList = new vertex_list();
  for (auto &v : *dpList)
    preserveDPList->push_back(v);
  for (auto &v : *dpList)
    initializeInheritedVertexAdjacentLabels(
        &slim::element::get<InheritedVertex>(v));

  auto key = makeDiscretePropagationListKey(preserveDPList);
  auto seniorDPList =
      discretePropagationListsOfInheritedVerticesWithAdjacentLabels->search(
          key);

  if (seniorDPList == NULL) {

    discretePropagationListsOfInheritedVerticesWithAdjacentLabels->insert(
        key, preserveDPList);
    isExisting = FALSE;
    return isExisting;
  } else {
    auto iteratorCell = std::begin(*preserveDPList);
    auto iteratorCellSenior = std::begin(*seniorDPList);

    while (iteratorCell != std::end(*preserveDPList)) {
      if (*iteratorCell != CLASS_SENTINEL) {
        auto &iVertex = slim::element::get<InheritedVertex>(*iteratorCell);
        auto &iVertexSenior =
            slim::element::get<InheritedVertex>(*iteratorCellSenior);

        unionDisjointSetForest(iVertex.equivalenceClassOfIsomorphism,
                               iVertexSenior.equivalenceClassOfIsomorphism);
      }

      iteratorCell = std::next(iteratorCell, 1);
      iteratorCellSenior = std::next(iteratorCellSenior, 1);
    }

    freePreserveDiscreteProapgationList(preserveDPList);

    isExisting = TRUE;
    return isExisting;
  }
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

Bool listMcKayInner(
    vertex_list *propagationListOfInheritedVertices,
    ConvertedGraph *cAfterGraph, int gapOfGlobalRootMemID,
    RedBlackTree__<KeyContainer__<vertex_list *>, vertex_list *>
        *discretePropagationListsOfInheritedVerticesWithAdjacentLabels) {
  Bool isUsefulBranch = TRUE;

  auto stabilizer = new vertex_list(*propagationListOfInheritedVertices);
  getStableRefinementOfConventionalPropagationList(stabilizer, cAfterGraph,
                                                   gapOfGlobalRootMemID);

  /*
  CHECKER("###### after stable refinement ######\n");
  std::cout << *stabilizer << std::endl;
  //*/

  auto beginSentinel = firstNonTrivialCell(stabilizer);

  if (beginSentinel == std::end(*stabilizer)) {
    isUsefulBranch =
        !insertDiscretePropagationListOfInheritedVerticesWithAdjacentLabelToTable(
            discretePropagationListsOfInheritedVerticesWithAdjacentLabels,
            stabilizer, cAfterGraph, gapOfGlobalRootMemID);
  } else {
    Bool isFirstLoop = TRUE;

    auto endSentinel = getNextSentinel(beginSentinel);
    auto sentinelCell =
        stabilizer->insert(std::next(beginSentinel, 1), CLASS_SENTINEL);

    for (auto iteratorCell = sentinelCell;
         std::next(iteratorCell, 1) != endSentinel;
         iteratorCell = std::next(iteratorCell, 1)) {
      auto splitCell = std::next(iteratorCell, 1);

      if (isNewSplit(sentinelCell, splitCell)) {
        stabilizer->splice(std::next(beginSentinel, 1), *stabilizer, splitCell);

        Bool isUsefulChild = listMcKayInner(
            stabilizer, cAfterGraph, gapOfGlobalRootMemID,
            discretePropagationListsOfInheritedVerticesWithAdjacentLabels);

        stabilizer->splice(std::next(iteratorCell, 1), *stabilizer, splitCell);

        if (isFirstLoop) {
          isFirstLoop = FALSE;
          if (!isUsefulChild) {
            isUsefulBranch = FALSE;
            break;
          } else {
            isUsefulBranch = TRUE;
          }
        }
      }
    }
  }

  delete (stabilizer);

  return isUsefulBranch;
}

vertex_list *listMcKay(vertex_list *propagationListOfInheritedVertices,
                       ConvertedGraph *cAfterGraph, int gapOfGlobalRootMemID) {
  if (propagationListOfInheritedVertices->empty()) {
    vertex_list *canonicalDiscreteRefinement =
        new vertex_list(*propagationListOfInheritedVertices);
    return canonicalDiscreteRefinement;
  } else {
    initializeDisjointSetForestsOfPropagationList(
        propagationListOfInheritedVertices);
    auto discretePropagationListsOfInheritedVerticesWithAdjacentLabels =
        new RedBlackTree__<KeyContainer__<vertex_list *>, vertex_list *>();

    classifyConventionalPropagationListWithAttribute(
        propagationListOfInheritedVertices, cAfterGraph, gapOfGlobalRootMemID);

    /*
    CHECKER("###### after attribute classifying ######\n");
    std::cout << *propagationListOfInheritedVertices << std::endl;
    //*/

    listMcKayInner(
        propagationListOfInheritedVertices, cAfterGraph, gapOfGlobalRootMemID,
        discretePropagationListsOfInheritedVerticesWithAdjacentLabels);

    vertex_list *canonicalDiscreteRefinement = new vertex_list();
    for (auto &v :
         *discretePropagationListsOfInheritedVerticesWithAdjacentLabels->begin()
              ->second)
      canonicalDiscreteRefinement->push_back(v);

    /*
    CHECKER("########### candidates of canonical discrete refinement
    ###########\n");
    redBlackTreeValueDump(discretePropagationListsOfInheritedVerticesWithAdjacentLabels,discretePropagationListDump);
    //*/

    freeRedBlackTreeWithValue(
        discretePropagationListsOfInheritedVerticesWithAdjacentLabels,
        freePreserveDiscreteProapgationList);

    return canonicalDiscreteRefinement;
  }
}

Bool checkIsomorphismValidity(unbound_vector<vertex_list *> *slimKeyCollection,
                              RedBlackTree__<KeyContainer__<vertex_list *>,
                                             CollectionInt> *McKayKeyCollection,
                              vertex_list *canonicalDiscreteRefinement,
                              long stateID) {
  Bool isValid = TRUE;

  if (stateID != 0) {
    auto key = makeDiscretePropagationListKey(canonicalDiscreteRefinement);
    CollectionInt seniorID = McKayKeyCollection->search(key) - 1;
    if (seniorID != -1) {
      if (stateID != seniorID) {
        fprintf(stdout, "stateID is wrong.\n");
        fprintf(stdout, "juniorStateID is %ld\n", stateID);
        fprintf(stdout, "seniorStateID is %ld\n", seniorID);
        isValid = FALSE;
        return isValid;
      }
    } else {
      McKayKeyCollection->insert(key, (stateID + 1));
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

vertex_list *trieMcKay(Trie *trie, DiffInfo *diffInfo, Graphinfo *cAfterGraph,
                       Graphinfo *cBeforeGraph) {
  int gapOfGlobalRootMemID =
      cBeforeGraph->globalRootMemID - cAfterGraph->globalRootMemID;
  int stepOfPropagation;
  Bool verticesAreCompletelySorted =
      triePropagate(trie, diffInfo, cAfterGraph, cBeforeGraph,
                    gapOfGlobalRootMemID, &stepOfPropagation);
  if (IS_DIFFERENCE_APPLICATION_MODE && verticesAreCompletelySorted) {
    /* printf("%s:%d\n", __FUNCTION__, __LINE__); */
    return new vertex_list();
  } else {

    vertex_list *propagationListOfInheritedVertices =
        makeConventionalPropagationList(trie, stepOfPropagation);

    /*
       CHECKER("###### before list propagate ######\n");

    //*/
    /* printf("%s:%d\n", __FUNCTION__, __LINE__); */
    //  std::cout << *propagationListOfInheritedVertices << std::endl;
    vertex_list *canonicalDiscreteRefinement =
        listMcKay(propagationListOfInheritedVertices, cAfterGraph->cv,
                  gapOfGlobalRootMemID);

    /*
       CHECKER("###### after list propagate ######\n");
       std::cout << *canonicalDiscreteRefinement << std::endl;
    //*/

    delete (propagationListOfInheritedVertices);

    return canonicalDiscreteRefinement;
  }
}
