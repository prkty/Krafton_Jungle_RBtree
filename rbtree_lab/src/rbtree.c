#include "rbtree.h"

#include <stdlib.h>

// 새 트리 생성 함수
rbtree *new_rbtree(void) {
  // tree 구조체 동적 할당
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  // NIL 노드 생성 및 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  if (nil == NULL)
    {
      free(p);
      return NULL;
    }
  nil -> color = RBTREE_BLACK;  // NIL 노드는 항상 Black
  nil->key = 0;
  nil->parent = NULL;
  nil->left = NULL;
  nil->right = NULL;
  // NIL 노드 생성 및 초기화

  // tree의 nil과 root를 nil 노드로 설정 (tree가 빈 경우 root는 nil노드여야 한다.)
  p -> nil = nil;
  p -> root = nil;

  return p;
}

// 왼쪽 회전
void left_rotation(rbtree *t, node_t *x) {
  // x가 위에 있는 부모이고, y는 x의 오른쪽 아래의 자식이다.
  node_t *y = x -> right;  // x의 오른쪽 자식은 노드 y 이다.
  x -> right = y -> left;  // y의 왼쪽 자식을 x의 오른쪽 자식으로 회전한다.

  if ( y -> left !=  t -> nil) {   // y의 왼쪽 자식이 비어 있지 않은 경우
    y -> left -> parent = x;       // y의 왼쪽 자식의 부모는 x가 된다.
  }

  y -> parent = x -> parent;       // x의 부모는 y의 부모가 된다.

  // 여기까지 y를 x자리로 올리기 위해 y의 자식을 x의 자식으로 처리하고 y를 x의 부모로 대체하는 과정이다. (y에 자식이 있을때)

  if (x -> parent == t -> nil) {  // x의 부모가 t는 nil(없을때)일때, 즉 x가 루트일때
    t -> root = y;                // y가 트리의 루트가 된다.
  }

  else if (x == x -> parent -> left) {   // x가 부모의 왼쪽 자식일때
    x -> parent -> left = y;             // x의 부모의 왼쪽 자식을 y로 대체
  }
  
  else {                         // 그외의 경우(x는 부모의 오른쪽 자식이었다
    x -> parent -> right = y;    // x의 부모의 오른쪽 자식을 y로 대체
  }

  y -> left = x;     // x를 y의 왼쪽 자식으로 둔다
  x -> parent = y;   // y룰 x의 부모로 둔다

  // 여기까지 x가 루트였으면 y를 루트로 두고, x가 부모가 있었으면 그 부모를 y의 부모로 바꾸는 과정이다.
  // 이후 마지막 두 문장은 x와 y의 부모, 자식 관계를 재정립한다.
}

// 오른쪽 회전
void right_rotation(rbtree *t, node_t *x) {
  // x가 위에 있는 부모이고, y는 x의 왼쪽 아래의 자식이다.
  node_t *y = x -> left;  // x의 왼쪽 자식은 노드 y 이다.
  x -> left = y -> right;  // y의 오른쪽 자식을 x의 왼쪽 자식으로 회전한다.

  if ( y -> right !=  t -> nil) {   // y의 오른쪽 자식이 비어 있지 않은 경우
    y -> right -> parent = x;       // y의 오른쪽 자식의 부모는 x가 된다.
  }

  y -> parent = x -> parent;       // x의 부모는 y의 부모가 된다.

  // 여기까지 y를 x자리로 올리기 위해 y의 자식을 x의 자식으로 처리하고 y를 x의 부모로 대체하는 과정이다. (y에 자식이 있을때)

  if (x -> parent == t -> nil) {  // x의 부모가 t는 nil(없을때)일때, 즉 x가 루트일때
    t -> root = y;                // y가 트리의 루트가 된다.
  }

  else if (x == x -> parent -> right) {   // x가 부모의 오른쪽 자식일때
    x -> parent -> right = y;             // x의 부모의 오른쪽 자식을 y로 대체
  }
  
  else {                         // 그외의 경우(x는 부모의 오른쪽 자식이었다
    x -> parent -> left = y;    // x의 부모의 왼쪽 자식을 y로 대체
  }

  y -> right = x;     // x를 y의 오른쪽 자식으로 둔다
  x -> parent = y;   // y룰 x의 부모로 둔다

  // 여기까지 x가 루트였으면 y를 루트로 두고, x가 부모가 있었으면 그 부모를 y의 부모로 바꾸는 과정이다.
  // 이후 마지막 두 문장은 x와 y의 부모, 자식 관계를 재정립한다.
}

// 삽입에서 넘긴 값을 토대로 수정 수행
void insert_fix(rbtree *t, node_t *new_node) {
  node_t *uncle; // 삼촌을 정의

while (new_node -> parent -> color == RBTREE_RED) {   // 삽입한 부모 노드의 색깔이 Red 일때까지 실행

    // 부모가 왼쪽일때 과정
    if (new_node->parent == new_node->parent->parent->left) {  // 새 노드의 부모가 조부모의 왼쪽일때
      uncle = new_node->parent->parent->right;      // 삼촌은 새 노드의 조부모의 오른쪽 자식임

      // Case 1
      if (uncle -> color == RBTREE_RED) {           // 삼촌의 색이 빨간색인 케이스
        new_node -> parent -> color = RBTREE_BLACK; // 새 노드의 부모는 검은색으로 지정
        uncle -> color = RBTREE_BLACK;              // 삼촌 또한 검은색으로 지정
        new_node -> parent -> parent -> color = RBTREE_RED;  // 조부모는 빨간색으로 지정
        new_node = new_node -> parent -> parent;    // 조부모까진 완벽하니, 조부모를 새 노드로 지정하고 재확인
      }
      else {
        if (new_node == new_node -> parent -> right) {  // 새 노드가 부모의 오른쪽에 있을때
          // Case 2
          new_node = new_node -> parent;               // 새 노드를 부모로 지정
          left_rotation(t, new_node);                  // 새 노드의 부모를 기준으로 왼쪽 로테이션 돌림
        }
          // Case 3(위치는 바꿨으니 색을 바꿔줘야함)
          new_node -> parent -> color = RBTREE_BLACK;  // 새 노드의 부모를 검은색으로 지정
          new_node -> parent -> parent -> color = RBTREE_RED;  // 조부모는 빨간색으로 지정
          right_rotation(t, new_node -> parent -> parent);     // 조부모 기준으로 오른쪽 로테이션 돌림
      }
    }

    // 위의 케이스와 다르게 부모가 오른쪽일때 과정
    else {
      uncle = new_node->parent->parent->left;      // 삼촌은 새 노드의 조부모의 왼쪽 자식임

      // Case 1
      if (uncle -> color == RBTREE_RED) {           // 삼촌의 색이 빨간색인 케이스
        new_node -> parent -> color = RBTREE_BLACK; // 새 노드의 부모는 검은색으로 지정
        uncle -> color = RBTREE_BLACK;              // 삼촌 또한 검은색으로 지정
        new_node -> parent -> parent -> color = RBTREE_RED;  // 조부모는 빨간색으로 지정
        new_node = new_node -> parent -> parent;    // 조부모까진 완벽하니, 조부모를 새 노드로 지정하고 재확인
      }
      else {
        if (new_node == new_node -> parent -> left) {  // 새 노드가 부모의 오른쪽에 있을때
          // Case 2
          new_node = new_node -> parent;               // 새 노드를 부모로 지정
          right_rotation(t, new_node);                  // 새 노드의 부모를 기준으로 오른쪽 로테이션 돌림
        }
          // Case 3(위치는 바꿨으니 색을 바꿔줘야함)
          new_node -> parent -> color = RBTREE_BLACK;  // 새 노드의 부모를 검은색으로 지정
          new_node -> parent -> parent -> color = RBTREE_RED;  // 조부모는 빨간색으로 지정
          left_rotation(t, new_node -> parent -> parent);     // 조부모 기준으로 왼쪽 로테이션 돌림
        }
      }
    }
  t -> root -> color = RBTREE_BLACK;  // 루트는 항상 검은색이어야하는 규칙(루트의 부모는 없기 때문에 색을 검은색으로 지정해도됨)
}

// 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 책에서는 새 노드가 z라고 나오는데, 
  // 해당 코드에선 z가 new_node로, new_node에 이미 키가 삽입되어 있다.

  // 노드생성
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  if (new_node == NULL) {
    return NULL;  // 또는 에러 메시지 출력 후 return
  }

  // 새로 추가할 노드 값 초기화
  new_node -> key = key;           // 새 노드안에 들어가는 값은 key
  // new_node -> color = RBTREE_RED;  // 무조건 새 노드 들어갈때 빨강 지정
  // new_node -> left = t -> nil;     // 새 노드의 왼쪽 자식 노드는 nil
  // new_node -> right = t -> nil;    // 새 노드의 오른쪽 자식 노드는 nil

  node_t *x = t -> root;  // x는 루트 노드에서 시작 (트리를 따라 내려감)
  // 현재 탐색 중인 노드 x(비교 대상)
  node_t *y = t -> nil;   // y는 x의 부모를 기억할 변수 (초기에는 nil)
  // y는 삽입될 위치의 부모 노드

  // 삽입 시작
  while (x != t -> nil) {   // x가 nil일 때 까지 내려간다(경계 NIL에 도달할 때까지 내려간다)
    y = x;   // 초기에는 y는 x이다. 
    // 밑으로 내려가면서 새 노드를 삽입할 적당한 위치를 찾는다.

    if (new_node -> key < x -> key) {  // 새노드의 키(값)가 x노드의 키(값)보다 작을때
      x = x -> left;         // x노드는 x의 오른쪽에 온다
    }
    else {                 // 반대의 경우
      x = x -> right;      // x노드는 x의 오른쪽에 온다
    }
  }
    new_node -> parent = y;  // y는 새 노드의 부모님으로 지정
    // 여기까지 탐색대상 x를 내리며 값이 작으면 왼쪽 자식을 탐색하고 크면 오른쪽 자식을 탐색한다.

    if (y == t -> nil) {     // y가 nil이면(트리에 아무것도 없었다)
      t -> root = new_node;  // 새 노드는 루트이다
    }
    else if (new_node -> key < y -> key) {  // 새 노드의 키가 y(부모)의 키보다 작으면
      y -> left = new_node;  // 새 노드는 y의 왼쪽 자식에 온다
    }
    else {  // 그외의 케이스, 새 노드가 y보다 크면
      y -> right = new_node; // 새 노드는 y의 오른쪽 자식에 온다
    }
    // 새노드를 삽입하기 위해 부모 y를 기준으로 키값을 비교하여 자식값에 넣는다.

    // 새노드에 왼쪽, 오른쪽 자식에 NIL을 추가, 새 노드는 빨강으로 지정
    new_node -> left = t -> nil;    
    new_node -> right = t -> nil;
    new_node -> color = RBTREE_RED;

  // 수정 작업 수행(트리와 새 노드값을 넘김)
  insert_fix(t, new_node);
    
  return new_node;
}


// RB 트리는 자식과 부모가 연계되어 있으므로 노드 자체의 메모리해제를 따로 해야한다.
// 왼쪽 자식 -> 오른쪽 자식 -> 자기자신 순으로 해제한다.
void free_node(rbtree *t, node_t *del) {
  if(del == t -> nil) {         // 받은 노드가 NULL이면 그냥 리턴
    return;
  }
  free_node(t, del -> left);    // 왼쪽 자식 메모리 해제하기 위해 재귀
  free_node(t, del -> right);   // 오른쪽 자식 메모리 해제하기 위해 재귀
  free(del);                    // 메모리 해제
}

// 모든 노드들 해제 후! NIL, RB 트리 구조체 해제
void delete_rbtree(rbtree *t) {
  free_node(t, t -> root);      // 루트를 시작점으로 모든 자식 노드를 순회하며 해제
  free(t -> nil);               // 공통의 NIL들 해제
  free(t);                      // 트리 구조체 해제
}

// 노드 검색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur = t -> root;    // cur은 루트부터 시작

  while(cur != t -> nil) {    // cur 현재 확인 중인 노드가 NIL 될때까지 실행
    if(cur -> key == key) {   // 현재 비교하는 cur과 키값이 같을때
      return cur;             // cur 반환
    }
    if(cur -> key < key) {    // 지금 확인중인 cur보다 키값이 클때
      cur = cur -> right;     // 오른쪽 자식으로 검색
    }
    else {
      cur = cur -> left;      // cur보다 키값이 작을때, 왼쪽 자식으로 검색
    }
  }
  if(cur == t -> nil) {      // cur이 없을때(트리가 없을때)
    return NULL;             // NULL 반환
  }
  return cur;                // 값 리턴
}

// 최솟값 탐색
node_t *rbtree_min(const rbtree *t) {
  node_t *cur = t -> root;    // cur은 루트부터 시작

  if(cur == t -> nil){        // 먼저 트리에 아무것도 없을 예외상황을 처리해야한다
    return NULL;              // 밑에 while문에 포함시키면 RB트리에서 맨밑이 항상 NIL이라 항상 NULL 나옴
  }

  node_t *min = NULL;
  while(cur != t -> nil) {    // NIL 만나기 직전까지 진행
    min = cur;                // NIL 만나기전 현재 노드를 기억
    cur = cur -> left;        // 최솟값으로 left로만 가야함
  }
  return min;                 // 값 리턴
}

// 최댓값 탐색
node_t *rbtree_max(const rbtree *t) {
  node_t *cur = t -> root;    // cur은 루트부터 시작

  if(cur == t -> nil){        // 먼저 트리에 아무것도 없을 예외상황을 처리해야한다
    return NULL;              // 밑에 while문에 포함시키면 RB트리에서 맨밑이 항상 NIL이라 항상 NULL 나옴
  }

  // 최댓값 검색
  node_t *max = NULL;
  while(cur != t -> nil) {    // NIL 만나기 직전까지 진행
    max = cur;                // NIL 만나기전 현재 노드를 기억
    cur = cur -> right;       // 최댓값으로 right로만 가야함
  }
  return max;                 // 값 리턴
}

// 삭제에서 노드가 하나의 자식만 가질 때, 두 자식을 가질때 후임자와 교체(부모와 후임자와 교체)
// u노드를 v로 대체함
void Transplant(rbtree *t, node_t * u, node_t *v) {
  if (u -> parent == t -> nil) {         // u의 부모가 NULL일때 (루트일때?)
    t -> root = v;                       // v(후임자)가 트리의 루트이다
  }

  else if (u == u -> parent -> left) {   // u가 u의 부모 왼쪽 자식과 같으면
    u -> parent -> left = v;             // u의 부모의 왼쪽 자식을 v로 대체
  }

  else {                                 // u가 u의 부모 오른쪽 자식과 같으면
  u -> parent -> right = v;              // u의 부모의 오른쪽 자식을 v로 대체
  }
  v -> parent = u -> parent;             // v의 부모는 u의 부모와 같다
}

// 특정노드 p의 후손을 찾음
node_t *minimum(rbtree *t, node_t *p) {  
  node_t *cur = p;                       // cur로 p의 후손을 찾는다.

  while(cur -> left != t -> nil) {       // cur 왼쪽 자식이 nil이 나올때까지 시행
    cur = cur -> left;                   // cur을 왼쪽 자식으로 보낸다.
  }
  return cur;   // cur을 리턴한다(erase로 보냄)
}

// 지우는 과정에서의 RB 트리 규칙에 따라 수정
void erase_fix(rbtree *t, node_t *x) {
  node_t *w = x;  // 형제 노드를 가리키기 위한 포인터 초기화 (임시)

  // x가 루트가 아니고, 검은색 노드일 때까지 반복 (균형 깨진 상태)
  while (x != t->root && x->color == RBTREE_BLACK) {

    // case: x가 부모의 왼쪽 자식일 경우
    if (x == x->parent->left) {
      w = x->parent->right;  // 형제 노드 w는 부모의 오른쪽 자식

      // case 1: 형제 w가 빨강일 때
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;               // 형제를 검정으로
        x->parent->color = RBTREE_RED;         // 부모는 빨강으로
        left_rotation(t, x->parent);           // 부모를 기준으로 좌회전
        w = x->parent->right;                  // 회전 후 w를 다시 지정
      }

      // case 2: 형제의 양쪽 자식이 모두 검정
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;                 // 형제를 빨강으로 칠함
        x = x->parent;                         // x를 부모로 올려서 fix 계속 진행
      }

      // case 3 or 4
      else {
        // case 3: 형제의 오른쪽 자식이 검정
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;       // 형제의 왼쪽 자식을 검정으로
          w->color = RBTREE_RED;               // 형제를 빨강으로
          right_rotation(t, w);                // 형제를 기준으로 우회전
          w = x->parent->right;                // w 업데이트
        }

        // case 4: 형제의 오른쪽 자식이 빨강 (case 3을 거쳐 오기도 함)
        w->color = x->parent->color;           // 형제는 부모의 색을 물려받고
        x->parent->color = RBTREE_BLACK;       // 부모는 검정
        w->right->color = RBTREE_BLACK;        // 형제의 오른쪽 자식도 검정
        left_rotation(t, x->parent);           // 부모 기준으로 좌회전
        x = t->root;                           // x를 루트로 설정하고 루프 종료
      }
    }

    // case: x가 부모의 오른쪽 자식일 경우 (위와 좌우 반대)
    else {
      w = x->parent->left;  // 형제 노드는 왼쪽 자식

      // case 1: 형제가 빨강일 때
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }

      // case 2: 형제 자식 둘 다 검정
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // case 3 or 4
      else {
        // case 3: 형제의 왼쪽 자식이 검정
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }

        // case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }
    }
  }

  // 루프가 끝나면 x의 색을 검정으로 (속성 회복)
  x->color = RBTREE_BLACK;
}


// y: 실제로 삭제되거나 트리 구조에서 제거될 노드
// x: erase_fix()에서 사용할 삭제된 자리를 채우는 노드
// y_origin_color: 삭제 전 노드의 색을 기억 → 검정이면 erase_fix()가 필요함
int rbtree_erase(rbtree *t, node_t *p) {
    node_t *y = p;     // 추후에 p를 따로 쓰기 위해 y로 노드 인자 추가
    node_t *x = p;     // 추후에 p를 따로 쓰기 위해 x로 노드 인자 추가
    color_t y_origin_color = y -> color;   // y 컬러를 나중에 쓰기 위해 y_origin_color로 저장
    if (p -> left == t -> nil) {           // 기준p의 왼쪽 자식이 NIL이면 수행
      x = p -> right;                      // x를 p의 오른쪽 자식이라고 지정
      Transplant(t, p, p -> right);        // p를 오른쪽 자식으로 바꾼다. 
    }
    else if (p -> right == t -> nil) {     // p의 오른쪽 자식인 NIL이라면
      x = p -> left;                       // x를 p의 왼쪽자식으로 지정
      Transplant(t, p, p -> left);         // p를 왼쪽 자식으로 바꾼다. 
    }
    else {                         
      y = minimum(t,p -> right);           // p를 오른쪽 자식으로 바꾼다.
      y_origin_color = y -> color;         // y의 원래 컬러를 저장한다
      x = y -> right;                      // x는 y의 오른쪽 자식이다.
    ////////// 해당 부분 문제 //////

    //   if (y->parent == p)
    //   x->parent = y;
    // else {
    //   Transplant(t, y, y->right);
    //   y->right = p->right;
    //   y->right->parent = y;
    //  }
    //   Transplant(t, p, y);
    //   y->left = p->left;
    //   y->left->parent = y;
    //   y->color = p->color;
    // }
    
    ////////// 수정전 ////////
    
      if (y != p -> right) {           // y가 트리에서 더 아래쪽인가?
        Transplant(t, y, y -> right);  // y를 오른쪽 자식으로 바꾼다.
        y -> right = p -> right;
        y -> right -> parent = y;
      }
      else {                   // 해당 else문을 잘못 들여쓰기 했다.
        x -> parent = y;       // x가 NIL인 경우 
      }
        Transplant(t, p, y);      // p를 그 후손인 y로 바꾼다.
        y -> left = p -> left;    // z의 왼쪽 자식을 y에 부여한다.
        y -> left -> parent = y;  // 왼쪽 자식이 없는 y
        y -> color = p -> color; 
    }
    
    //////////////////////////////
    if (y_origin_color == RBTREE_BLACK) {   // 레드 블랙 위반이 발생한 경우
      erase_fix(t, x);                      // 수정한다.
    }
  return 0;
}

// 키값이 주어진 n 크기까지의 배열의 개수를 출력(중위순회)
// 내부에서 쓰는 재귀함수로 실제 트리를 순회하며 데이터를 배열에 넣음
void to_array(const rbtree* t, key_t *arr, const size_t n, node_t* cur, int* count) {
  if(*count >= n || cur == t -> nil) return;   
  // count는 지금까지 배열에 저장한 key의 개수, n은 배열에 담을 수 있는 최대 개수 
  // 그러므로count가 주어진 n보다 커지거나 같아지면 바로 리턴
  // 맨마지막인 NIL까지 내려가서 순회 종료

  // 중위순회
  to_array(t, arr, n, cur -> left, count);   // 왼쪽 자식 맨끝부터 시작
  if(*count < n) {                           // n횟수만큼 카운트가 진행
    arr[*count] = cur -> key;                // 배열에 현재 진행중인 키를 저장
    *count += 1;                             // 배열에 저장한 횟수 +1
  }
  to_array(t, arr, n, cur -> right, count);  // 오른쪽을 순회하고 끝냄
}

// 사용자가 호출하는 함수 역할로 외부에선 해당 함수만 호출(트리, 배열, 사이즈 n만 호출해 사용)
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int count = 0;      // count 0으로 초기화
  to_array(t, arr, n, t -> root, &count);  
  // &count인 이유는 하단의 *count가 포인터로 인자를 줄려면 & 주소로 보내줘야한다.(값 변경을 반영)

  return 0;    // 값 리턴
}