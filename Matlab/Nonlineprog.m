function f = fun1( x )      %fΪ���ؽ����fun1Ϊ��������xΪ�Ա��������������
    f=sum(x.^2)+8               %����
end

function [g,h] = fun2( x )              %�������x��fun2��������gΪ�����Բ���ʽ��hΪ�����Ե�ʽ
    g=[-x(1)^2+x(2)-x(3)^2,x(1)+x(2)^2+x(3)^2-20];   %�����Բ���ʽ�Ľ������g 
    h=[x(2)+2*x(3)^2-3,-x(1)-x(2)^2+2];              %�����Ե�ʽ�Ľ������h
end

clear
x0=[1,1,1];
lb=[0,0,0];
ub=[inf,inf,inf];
A=[];b=[];Aeq=[];beq=[];
[x,fval]=fmincon(@fun1,x0,A,b,Aeq,beq,lb,ub,@fun2)