%% Lagrange Interpolation Polynomial
%
% If you have a set of N points on a cartesian plane, there will
% always exist an N-1th order polynomial of the form 
%  y = a_0 + a_1.x + a_2.x^2 + ... a_n-1.x^(n-1)
% which passes through all the points.  Lagrange came up with a 
% neat approach to finding this polynomial, which is to construct 
% a set of `basis' polynomials which are zero at all the specified 
% points except for one, then scale and add them to match all the 
% control points.  LAGRANGEPOLY returns this polynomial, defined by 
% the polynomial coefficients (a_0 .. a_n-1 above), arranged in the 
% same vector form used by Matlab builtins such as POLYVAL.
%
% LAGRANGEPOLY optionally returns the x and y co-ordinates of all 
% the extrema and points of inflection of the polynomial too, since 
% these are easily found from the polynomial form.
%
% Lagrange interpolation has generally been replaced by spline 
% fitting these days; see the SPLINE function.
%
% For more information on Lagrange interpolation, there is a good 
% explanation on the Mathworld web site, 
% http://mathworld.wolfram.com/LagrangeInterpolatingPolynomial.html

%% Example
%
% Find the polynomial going through 8 evenly-spaced points, 
% then plot the curve, along with the control points (red circles,
% to check the curve does indeed go through them) and the 
% extrema (blue dots).
%
% Also, compare to SPLINE (dotted green line), which also 
% passes through the points without such extreme deviation
% (but is not in the form of a single polynomial).
clc
% clear
close all
A = [109.48141 5.73869;
118.78291 5.68338;
127.978 5.6535;
137.14038 5.63612;
146.10465 5.63461;
154.98326 5.65129;
163.80103 5.68479;
172.54918 5.73521;
181.24271 5.80281;
189.9127 5.8865;
198.46533 5.98923;
206.96054 6.10715;
215.32832 6.24573;
223.60695 6.4057;
231.81181 6.586;
239.98952 6.79507;]
A1 = A(1:16);
A2 = A(17:32);
% X = [1 2 3 4 5 6 7 8];
% Y = [0 1 0 1 0 1 0 1];
X = [A(1), A(1+5),A(1+10), A(16)]
Y = [A(17), A(17+5),A(17+10), A(32)]
[P,R,S] = lagrangepoly(X,Y);

P2 = [P(1:4)]

xx = 50  : 300;
plot(xx,polyval(P,xx),X,Y,'or',R,S,'.b',xx,spline(X,Y,xx),'--g',A1,A2,'y',xx,polyval(P2,xx),'c')
grid
axis([50 300 5 7])
P
R
S
P2

