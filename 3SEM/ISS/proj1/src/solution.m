% ISS 2014 - PROJEKT - Filip Jezovica

% nacitanie obrazka
I=imread('xjezov01.bmp');


% uloha 1 - zaostrenie
matica1 = [-0.5 -0.5 -0.5; -0.5 5 -0.5 ; -0.5 -0.5 -0.5];
I1 = imfilter(I,matica1);
imwrite(I1, 'step1.bmp');


% uloha 2 - preklapanie zaostreneho obrazka
I2 = fliplr(I1);
imwrite(I2, 'step2.bmp');


% uloha 3 - medianovy filter
I3 = medfilt2(I2, [5 5]);
imwrite(I3,'step3.bmp');


% uloha 4 - rozmazanie
matica4 = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1] / 49;
I4 = imfilter(I3,matica4);
imwrite(I4,'step4.bmp');


% uloha 5 - chyby v obraze
U5=double(fliplr(I4));
Ipom=double(I);
CHYBY = 0;

for (x=1:512)
	for (y=1:512)
		CHYBY=CHYBY+abs(Ipom(x,y)-U5(x,y));
	end;
end;

CHYBY=CHYBY/512/512


% uloha 6 - roztiahnutie histogramu
MAX=255; % maximalny rozsah

maxi=double(max(max(I4))); 
mini=double(min(min(I4)));

I5=imadjust(I4,[mini/MAX maxi/MAX],[0; 1]);
I5=uint8(I5);
imwrite(I5,'step5.bmp');


% uloha 7 - stredné hodnoty + smerodatne odchylky
mean_no_hist=mean(mean(double(I4)))
std_no_hist=std2(double(I4))
mean_hist=mean(mean(double(I5)))
std_hist=std2(double(I5))


% uloha 8 - kvantizacia obrazu
N=2;
a=0;
b=255;
I5POM = double(I5);
I6 = zeros(512,512);

for k=1:512
	for l=1:512
		I6(k,l) = round(((2^N)-1)*(I5POM(k,l)-a)/(b-a))*(b-a)/((2^N)-1) + a;
	end
end

I6=uint8(I6);
imwrite(I6,'step6.bmp');