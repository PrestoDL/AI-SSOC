Rust

메모리 안정성에 치중하면서도 성능은 딱히 빠지는 부분이 없음

소유권이나 라이프타임, 동적할당된 데이터를 자동으로 없애는 등의 특징이 있으며

이는 OS에서 발생하는 race condition이나 deadlock의 발생을 최소화할 수 있음



// memory







let // 변수 생성.

i32 // int 타입. 부호 없는 타입으로 u32도 가능하고, 8bit 16bit 등 다양하게 부여 가능 (let a: i32 = 100;)
f32 // float 타입.

char // char타입. 작은따옴표 'a'로 선언 가능. 4byte 고정
&str // 문자열 포인터 타입. 큰따옴표 "aaa"로 선언 가능. C에서 취급하는 문자열과 비슷한 듯.
String // string타입. 힙에 할당된 문자열 데이터 전체를 소유하는 타입. "aaa".to_string()을 붙이거나 String::from("aaa")로 선언 가능.
참고로 &String형태의 데이터는 &str로 자동 변환해버림.
mutable한 string의 경우 push, pop 가능. string은 힙에 할당된 만큼 capacity가 변경될 수 있다.



array // 파이썬의 리스트와 유사하나, 타입과 크기의 묶음이 하나의 타입으로 취급 (let arr: [&str; 3] = ["aa", "bb", "cc"];)
같은 타입의 array끼리만 비교 가능. array 접근법도 동일한데, (arr[1] == bb) 인덱싱 범위를 벗어나는 경우 에러가 나며 .get(3)등으로 접근하면 오류가 나지 않음

vec // 벡터. 타입을 지정해주고 선언 가능.

```
let mut vv: Vec<String> = Vec::new();
```

아니면 매크로 사용도 가능하다

```
let vv: Vec<String> = vec!["aa", "bb"];
```

굳이 타입을 지정하지 않아도 벡터에 뭔가 넣어주면 해당 타입으로 자동 지정된다. push나 pop 메소드 사용가능
tuple // 튜플. ()로 묶이며 벡터와는 달리 원소가 같은 타입일 필요는 없음.

```
let tt: (i32, &str, Vec<i32>) = (8, "apple", vec![1, 2, 3]);
```

처럼 선언 가능
튜플 접근은 (tt.0 == 8)과 같음. 같은 데이터 묶음의 튜플이 벡터 안에 들어갈 수도 있음.
enum // 열거형. 다양한 상태를 표현 가능하고 처리하는 집합이라고 봐도 될듯. method 사용 가능.
option // NULL 대용 열거형.
struct // 구조체. 튜플로 지정해버리거나

```
struct tt(u32, u32, char);
```

기존 알던 방식으로 지정해도 된다

```
struct ss{aa:  u32, bb: u32, cc: char}
```

지정해준 대로 접근도 하면 된다(tt.2, ss.aa 등).

또한 기존 알던 방식대로 지정할 경우 변수 멤버로 들어가는 변수 이름이 같으면 한 번씩만 써줘도 선언이 된다

const // C에서의 define과 유사함. 다른 점이라면, 데이터 타입 지정이 가능함.
static // const와 유사하나 해당 값을 모두가 쓴다는 차이점이 존재.



print! // printf와 유사. 출력에 변수가 포함될 경우 {}로 둠
println! // print!에서 끝에 \n가 포함됨.
{}안에 번호를 부여해 활용할수도 있으며, 비어있을 경우 순서대로 {0}, {1},... 초기값으로 취급
{}안에 형태를 나타내준다면 ({:p} 포인터, {:b} 바이너리) 해당하는 형태로 출력해줌
오로지 문자열만을 출력하는 상황이라면 r#"a\a\\a"#와 같은 형태로 출력 (raw text)



loop // while(True)와 크게 다를 건 없어보인다. 물론 break;로 나올 수 있다
if // C에서와 마찬가지로 else 등도 사용가능. &&, ||를 이용한 조건 적용 가능
match // switch:case와 유사함. match로 열어놓고 0 => ~~~, 1 => ~~~, _ => ~~~로 처리. _는 default. break를 자동으로 걸어줌
조사하는 데이터는 i32뿐 아니라 tuple 등도 가능하며 tuple의 세부사항을 기준으로도 조건을 줄 수 있음



fn // 함수. 함수 내용물 중 ;이 없는 내용이 return됨. 그러나 main함수는 empty tuple을 반환해야만 함
ie) fn plus(a: i32, b: i32) -> i32 { a + b } // a+b의 합을 i32형식으로 반환하는 함수 plus 선언

let {~~~} // 익명함수. 간단한 익명 함수로 표현 가능. 물론 {}안에 함수가 구현되기에 return될 값에 ;가 붙어선 안됨



mutability // 가변 표시. mut로 표시. mut가 없을 경우, 변수는 기본적으로 immutable(선언 이후 변경 불가).
shadowing // 변수 재선언. let으로 변수를 다시 선언해줄 수 있으며 데이터 타입과 내용 변경 가능
reference // C의 포인터. &으로 표시. *으로 참조 가능. 마찬가지로 포인터의 포인터(&&&..)도 가능
크기가 정해지지 않은 데이터는 heap에 저장되고, 크기가 지정된 데이터(포인터 등)는 stack에 저장된다
mutable 레퍼런스가 이미 있는 시점에서 해당 데이터를 참조하는 다른 레퍼런스를 생성할 경우, 전에 생성된 mutable 레퍼런스의 수명이 끝난다

ownership // 함수를 호출할 경우 지정한 변수들의 소유권을 해당 함수에게로 넘김.(move semantics)
이 때문에, 함수 호출에 원본 데이터 대신 reference를 활용하게 됨. 주로 String에 해당 (clone type)
혹은 원본 데이터.clone()을 사용하면 원본의 복사본 데이터가 들어가 소유권 문제 해결 가능.
ownership과 관련한 문제는 i32와 같은 단순한 primitive type에서는 딱히 문제가 없음.(copying type)

box // 동적 메모리 할당. 주로 외부 공유가 필요없는 경우 사용

```
let mut a = Box::new(10); // a에 10과 같은 형식(여기서는 i32)의 데이터를 힙에 할당
*a = 20; // 동적할당된 데이터 접근 시
```

놀랍게도 동적할당된 데이터는 사용되지 않는 시점에 알아서 free된다.

rc // 레퍼런스 카운팅 포인터. box에 비해 공유가 가능하다는 차이점이 존재하나 기본적으로 불변성을 가져 수정이 불가능하다. rc 또한 참조되지 않는(참조 횟수가 0인) 데이터는 삭제해버린다.

refcell // rc 사용 시 임시로 가변성을 부여하는 자료형

lifetime // 변수 대여 시 대여 기간을 명시적으로 지정하기 위함.