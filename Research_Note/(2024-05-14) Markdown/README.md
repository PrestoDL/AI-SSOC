# Reset

 Flip-Flop 회로를 설계함에 있어 reset 신호는 필수적으로 구성되어야 하는 요소이며, 이러한 reset 신호를 구현하는 방법에는 Flip-Flop에 입력되는 clock과의 동기화 여부에 따라 synchronous reset과 asynchronous reset으로 구분된다. 

 이번 chapter에서는 이러한 두 reset의 차이점과 구현 방법에 대해 알아보도록 한다.



## Synchronous reset

 Synchronous reset 구조의 Flip-Flop 회로는 Flip-Flop에 입력되는 clock 소스와 동기된 상태로 reset 신호가 구동하는 것을 의미하며, 그림01과 같은 회로적인 구조를 가지게 된다.  

![01](./images\01.jpg 'Example of synchronous reset Flip-Flop')

```
// cft: 'Example of synchronous reset Flip-Flop'
reg     Q;

always @(posedge clk)
begin
    if(!reset)
        Q   <= 1'b0;
    else
        Q   <= select ? din_B : din_A;
end
// cft
```

 그림02는 synchronous reset Flip-Flop의 reset 신호에 대한 출력 Q 가 어떻게 구동되는 보여주는 waveform이 되겠다. reset 신호가 assert 되어도 positive edge clock 신호에 동기되어 Flip-Flop의 출력 Q가 reset value를 출력하는 것을 알 수 있다. 

![02](./images\02.jpg 'Waveform of synchronous reset Flip-Flop')

## Asynchronous reset

 Asynchronous reset 구조의 Flip-Flop 회로는 Flip-Flop에 입력되는 clock 소스와 동기되지 않은 비동기 상태로 reset 신호가 구동하는 것을 의미하며, 그림03과 같은 회로적인 구조를 가지게 된다.  

![03](./images\03.jpg 'Example of asynchronous reset Flip-Flop')

```
// cft: 'Example of asynchronous reset Flip-Flop'
reg     Q;

always @(posedge clk or negedge reset)
begin
    if(!reset)
        Q   <= 1'b0;
    else
        Q   <= select ? din_B : din_A;
end
// cft
```

 그림04는 asynchronous reset Flip-Flop의 reset 신호에 대한 출력 Q 가 어떻게 구동되는 보여주는 waveform이 되겠다. reset 신호가 assert 되는 시점에 Flip-Flop의 clock 신호와 관계없이 바로 출력 Q가 reset value를 출력하는 것을 알 수 있다.

![04](./images\04.jpg 'Waveform of asynchronous reset Flip-Flop')