#!/usr/bin/python
# -*- coding: utf-8 -*-
from Tkinter import Frame, Tk, BOTH, Text, Menu, END, Button,RIGHT,LEFT,TOP,BOTTOM,RAISED,SUNKEN,RIDGE,Y,X,FLAT,INSERT,Entry,Label,LabelFrame,StringVar,OptionMenu,Radiobutton,Checkbutton,IntVar,Listbox
from numpy import *
import tkFileDialog 
import matplotlib 
import matplotlib.figure
import matplotlib.backends.backend_tkagg

from scipy import signal



class Example(Frame):
  
	def __init__(self, parent):
		Frame.__init__(self, parent)   
		self.parent = parent        
		self.initUI()
        
	def initUI(self):
      
		self.parent.title("IAF CALC 0.01")
		self.pack(fill=BOTH, expand=1)
        
		self.configure(background='white')
		
		frameTOP = Frame(self);
		frameTOP.config(bg="white")
		frameTOP.pack(side=TOP)
		
		frameFILES = Frame(frameTOP)
		frameFILES.pack(side=LEFT,padx=10)
		
		# --- BUTTON FOR FILE 1 --- #
		frameF1 = LabelFrame(frameFILES,text="Eyes open file:", relief=FLAT, borderwidth=1,background="white")
		frameF1.pack(fill=X, expand=1)
		self.nameF1 = Entry(frameF1,width=50)
		self.nameF1.config(bg="lightgray")
		self.nameF1.pack(side=LEFT)
		self.nameF1.delete(0, END)
		self.nameF1.insert(0, "")
		self.buttonLoadFile1 = Button(frameF1, text="...",command=self.askOpenFile1)
		self.buttonLoadFile1.pack(side=LEFT,padx=5,pady=5)
		# ----------------------- #
		
		
		# --- BUTTON FOR FILE 2 --- #
		frameF2 = LabelFrame(frameFILES,text="Eyes closed file:", relief=FLAT, borderwidth=1,background="white")
		frameF2.pack(fill=X, expand=1)
		self.nameF2 = Entry(frameF2,width=50)
		self.nameF2.config(bg="lightgray")
		self.nameF2.pack(side=LEFT)
		self.nameF2.delete(0, END)
		self.nameF2.insert(0, "")
		self.buttonLoadFile2 = Button(frameF2, text="...",command=self.askOpenFile2)
		self.buttonLoadFile2.pack(side=LEFT,padx=5,pady=5)
		# ----------------------- #
		
		# --- BUTTON FOR FILE OUTPUT --- #
		frameO = LabelFrame(frameFILES,text="Output directory:", relief=FLAT, borderwidth=1,background="white")
		frameO.pack(fill=X, expand=1)
		self.nameO = Entry(frameO,width=50)
		self.nameO.config(bg="lightgray")
		self.nameO.pack(side=LEFT)
		self.nameO.delete(0, END)
		self.nameO.insert(0, "")
		self.buttonSelectOutput = Button(frameO, text="...",command=self.askOutputDirectory)
		self.buttonSelectOutput.pack(side=LEFT,padx=5,pady=5)
		# -------------------------------#
		# self.pack()
		# self.pack(fill=Y, expand=1)
		
		# ---------- PSD PARAMETER SELECTION ---------- #
		framePARAM = Frame(frameTOP)
		framePARAM.config(bg="white")
		framePARAM.pack(side=LEFT,fill=X)
		
		
		frame = LabelFrame(framePARAM,text="PSD Parameters", relief=RIDGE, borderwidth=1,background="white")
		frame.pack(fill=BOTH,expand=1,side=TOP)
		
		wFs = Label(frame, text="Fs:",bg="white")
		wFs.pack(side=LEFT)
		self.inputFs = Entry(frame,width=5)
		self.inputFs.pack(side=LEFT,padx=5)
		self.inputFs.delete(0, END)
		self.inputFs.insert(0, "500")
		
		wWS = Label(frame, text="Window size:",bg="white")
		wWS.pack(side=LEFT)
		self.inputWinSize = Entry(frame,width=5)
		self.inputWinSize.pack(side=LEFT,padx=5)
		self.inputWinSize.delete(0, END)
		self.inputWinSize.insert(0, "1024")
		
		wOL = Label(frame, text="Overlap:",bg="white")
		wOL.pack(side=LEFT)
		self.inputOverlap = Entry(frame,width=5)
		self.inputOverlap.pack(side=LEFT,padx=5)
		self.inputOverlap.delete(0, END)
		self.inputOverlap.insert(0, "512")
		
		wWT = Label(frame, text="Window function:",bg="white")
		wWT.pack(side=LEFT)
		
		variable = StringVar(frame)
		variable.set("Hamming") # default value
		self.inputWinType = OptionMenu(frame, variable, "Hamming", "Bartlett", "Blackman","Hanning","None")
		self.inputWinType.config(bg = "white",width=10)
		self.inputWinType.pack(side=LEFT)
		
		buttonRun = Button(frame, text="GO!",command=self.goTime)
		buttonRun.pack(side=RIGHT)
		
		# Channel selector
		frameCh = LabelFrame(framePARAM,text="Channels", relief=RIDGE, borderwidth=1,background="white")
		frameCh.pack(fill=BOTH,expand=1,side=TOP)
		
		self.EOch1 = IntVar()
		self.inputEOch1 = Checkbutton(frameCh, text="1",variable=self.EOch1,bg="white")
		self.inputEOch1.pack(side=LEFT)
		
		self.EOch2 = IntVar()
		self.inputEOch2 = Checkbutton(frameCh, text="2",variable=self.EOch2,bg="white")
		self.inputEOch2.pack(side=LEFT)
		
		self.EOch3 = IntVar()
		self.inputEOch3 = Checkbutton(frameCh, text="3",variable=self.EOch3,bg="white")
		self.inputEOch3.pack(side=LEFT)
		
		self.EOch4 = IntVar()
		self.inputEOch4 = Checkbutton(frameCh, text="4",variable=self.EOch4,bg="white")
		self.inputEOch4.pack(side=LEFT)
		
		self.EOch5 = IntVar()
		self.inputEOch5 = Checkbutton(frameCh, text="5",variable=self.EOch5,bg="white")
		self.inputEOch5.pack(side=LEFT)
		
		self.EOch6 = IntVar()
		self.inputEOch6 = Checkbutton(frameCh, text="6",variable=self.EOch6,bg="white")
		self.inputEOch6.pack(side=LEFT)
		
		self.EOch7 = IntVar()
		self.inputEOch7 = Checkbutton(frameCh, text="7",variable=self.EOch7,bg="white")
		self.inputEOch7.pack(side=LEFT)
		
		self.EOch8 = IntVar()
		self.inputEOch8 = Checkbutton(frameCh, text="8",variable=self.EOch8,bg="white")
		self.inputEOch8.pack(side=LEFT)
		
		# IAF Calculation parameters
		
		frameIAF = LabelFrame(framePARAM,text="IAF Search Limits", relief=RIDGE, borderwidth=1,background="white")
		frameIAF.pack(fill=BOTH,expand=1,side=TOP)
		
		labelLowBound = Label(frameIAF, text="Lower limit (Hz):",bg="white")
		labelLowBound.pack(side=LEFT)
		self.inputLowBound = Entry(frameIAF,width=5)
		self.inputLowBound.pack(side=LEFT,padx=5)
		self.inputLowBound.delete(0, END)
		self.inputLowBound.insert(0, "7")
		
		labelUpBound = Label(frameIAF, text="Upper limit (Hz):",bg="white")
		labelUpBound.pack(side=LEFT)
		self.inputUpBound = Entry(frameIAF,width=5)
		self.inputUpBound.pack(side=LEFT,padx=5)
		self.inputUpBound.delete(0, END)
		self.inputUpBound.insert(0, "14")
		
		self.GaussVar = IntVar()
		self.inputGauss = Checkbutton(frameIAF, text="Gauss",variable=self.GaussVar,bg="white")
		self.inputGauss.pack(side=LEFT)
		
		buttonRun = Button(frameIAF, text="IAF!",command=self.calculateIAF)
		buttonRun.pack(side=RIGHT)
		
		self.pack()
		
		# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		# END OF TOP FRAME
		# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
		
		
		# my variables
		self.chSelect = 0
		self.chOther = 0
		
		self.P1 = ndarray(1)
		self.f1 = ndarray(1)
		
		self.P2 = ndarray(1)
		self.f2 = ndarray(1)
		
		self.filename1 = "..."
		self.filename2 = "..."
		
		self.IAF = 10
		
		self.doGauss = True
		# FIGURE STUFF !!!
		self.Pmax = 0;
		self.Pmin = 0;
		self.Dmax = 0;
		self.Dmin = 0;
		
		frameBOTTOM = Frame(self)
		frameBOTTOM.config(bg="white")
		frameBOTTOM.pack(side=BOTTOM,pady=10)
		
		frameFig = LabelFrame(frameBOTTOM,text="Spectrum", relief=RIDGE, borderwidth=1,background="white")
		frameFig.pack(fill=X, expand=1,side=LEFT,padx=10)
		
		self.fig1 = matplotlib.figure.Figure(figsize=(7,3), dpi=100) #,frameon=False)
		self.fig1.set_facecolor('white')
		self.fig = matplotlib.backends.backend_tkagg.FigureCanvasTkAgg(self.fig1,master=frameFig)
		self.a1 = self.fig1.add_subplot(121)
		self.a2 = self.fig1.add_subplot(122)
		self.fig.show()
		self.fig.get_tk_widget().pack(side=BOTTOM)
		
		
		frameConfig = LabelFrame(frameBOTTOM,text="Filter configuration", relief=RAISED, borderwidth=1,background="white")
		frameConfig.pack(fill=BOTH, expand=1,side=RIGHT,padx=10)
		
		frameIAF = LabelFrame(frameConfig,text="Individual Alpha Frequency (IAF)")
		frameIAF.config(bg="white")
		frameIAF.pack(expand=1,side=TOP,padx=10)
		
		
		self.inputIAF = Entry(frameIAF,width=5)
		self.inputIAF.pack(side=LEFT,padx=5)
		self.inputIAF.delete(0, END)
		self.inputIAF.insert(0, "0")
		
		self.buttonWriteDefault = Button(frameIAF, text="Update Filters",command=self.updateFilters)
		self.buttonWriteDefault.pack(side=LEFT,padx=5,pady=5)

		
		frameFilters = LabelFrame(frameConfig,text="Filters", relief=RAISED, borderwidth=1,background="white")
		frameFilters.pack(fill=X, expand=1,side=TOP)
		
		# THETA FRAME
		frameTheta = LabelFrame(frameFilters,text="Theta", relief=RAISED, borderwidth=1,background="white")
		frameTheta.pack(expand=1,side=TOP,pady=5,padx=5)
		
		self.inputThetaLow = Entry(frameTheta,width=8)
		self.inputThetaLow.pack(side=LEFT,padx=5,pady=5)
		self.inputThetaLow.delete(0, END)
		self.inputThetaLow.insert(0, "0")
		
		self.inputThetaHigh = Entry(frameTheta,width=8)
		self.inputThetaHigh.pack(side=LEFT,padx=5,pady=5)
		self.inputThetaHigh.delete(0, END)
		self.inputThetaHigh.insert(0, "0")
		
		# BETA FRAME
		frameBeta = LabelFrame(frameFilters,text="Beta", relief=RAISED, borderwidth=1,background="white")
		frameBeta.pack(expand=1,side=TOP,pady=5,padx=5)
		
		self.inputBetaLow = Entry(frameBeta,width=8)
		self.inputBetaLow.pack(side=LEFT,padx=5,pady=5)
		self.inputBetaLow.delete(0, END)
		self.inputBetaLow.insert(0, "0")
		
		
		self.inputBetaHigh = Entry(frameBeta,width=8)
		self.inputBetaHigh.pack(side=LEFT,padx=5,pady=5)
		self.inputBetaHigh.delete(0, END)
		self.inputBetaHigh.insert(0, "0")
		
		
		# SMR FRAME
		frameSMR = LabelFrame(frameFilters,text="SMR", relief=RAISED, borderwidth=1,background="white")
		frameSMR.pack(expand=1,side=TOP,pady=5,padx=5)
		
		self.inputSMRLow = Entry(frameSMR,width=8)
		self.inputSMRLow.pack(side=LEFT,padx=5,pady=5)
		self.inputSMRLow.delete(0, END)
		self.inputSMRLow.insert(0, "0")
		
		
		self.inputSMRHigh = Entry(frameSMR,width=8)
		self.inputSMRHigh.pack(side=LEFT,padx=5,pady=5)
		self.inputSMRHigh.delete(0, END)
		self.inputSMRHigh.insert(0, "0")
		
		
		frameButtons = LabelFrame(frameConfig,text="Commands", relief=RAISED, borderwidth=1,background="white")
		frameButtons.pack(expand=1,side=BOTTOM)
		
		self.buttonWriteConfig = Button(frameButtons, text="Write Filters",command=self.writeFilters)
		self.buttonWriteConfig.pack(side=LEFT,padx=5,pady=5)
		
		self.buttonWriteDefault = Button(frameButtons, text="Reset to Defaults",command=self.resetIAFtoDefault)
		self.buttonWriteDefault.pack(side=LEFT,padx=5,pady=5)
		
		# self.buttonVisualize = Button(frameButtons, text="VIS",command=self.resetIAFtoDefault)
		# self.buttonVisualize.pack(side=LEFT,padx=5,pady=5)
		
		self.buttonPrintFig = Button(frameButtons, text="Print Figure",command=self.printFigureToFile)
		self.buttonPrintFig.pack(side=LEFT,padx=5,pady=5)
		
	def getChannelList(self):
		# Initialize 
		self.chSelect = asarray([])
		self.chOther = asarray([])
		
		if self.EOch1.get():
			self.chSelect = append(self.chSelect,0)
		else:
			self.chOther = append(self.chOther,0)
		
		if self.EOch2.get():
			self.chSelect = append(self.chSelect,1)
		else:
			self.chOther = append(self.chOther,1)
			
		if self.EOch3.get():
			self.chSelect = append(self.chSelect,2)
		else:
			self.chOther = append(self.chOther,2)	
		
		if self.EOch4.get():
			self.chSelect = append(self.chSelect,3)
		else:
			self.chOther = append(self.chOther,3)
			
		if self.EOch5.get():
			self.chSelect = append(self.chSelect,4)
		else:
			self.chOther = append(self.chOther,4)
			
		if self.EOch6.get():
			self.chSelect = append(self.chSelect,5)
		else:
			self.chOther = append(self.chOther,5)
		
		if self.EOch7.get():
			self.chSelect = append(self.chSelect,6)
		else:
			self.chOther = append(self.chOther,6)
			
		if self.EOch8.get():
			self.chSelect = append(self.chSelect,7)
		else:
			self.chOther = append(self.chOther,7)
			
	def updateFilters(self):
		
		# SET THETA
		self.inputThetaLow.delete(0,END)
		self.inputThetaLow.insert(0,"%.2f" % (float(self.inputIAF.get())*0.4))
		self.inputThetaHigh.delete(0,END)
		self.inputThetaHigh.insert(0,"%.2f" % (float(self.inputIAF.get())*0.6))
		# SET BETA
		self.inputBetaLow.delete(0,END)
		self.inputBetaLow.insert(0, "%.2f" % (float(self.inputIAF.get())*1.2))
		self.inputBetaHigh.delete(0,END)
		self.inputBetaHigh.insert(0,25)
		# SET SMR
		self.inputSMRLow.delete(0,END)
		self.inputSMRLow.insert(0,"%.2f" % (float(self.inputIAF.get())*1.2))
		self.inputSMRHigh.delete(0,END)
		self.inputSMRHigh.insert(0,"%.2f" % (float(self.inputIAF.get())*1.5))
		
		
	def resetIAFtoDefault(self):
		self.inputIAF.delete(0, END)
		self.inputIAF.insert(0, "10")
		self.updateFilters()
		
	def calculateIAF(self):
		self.getChannelList()
		print "LOLOL calculating IAF"
		m1 = 20*log10(mean(self.P1[self.chSelect.astype(int),:],axis=0))
		m2 = 20*log10(mean(self.P2[self.chSelect.astype(int),:],axis=0))
		d = m2-m1
		
		if self.GaussVar.get():
			
			# print d.shape
			# print gauss_signal.shape
			d_gauss = d[bitwise_and(self.f1>int(self.inputLowBound.get()),self.f1<int(self.inputUpBound.get()))]
			gauss_signal = signal.gaussian(d_gauss.shape[0],1)
			d_gauss = d_gauss * gauss_signal
			d[bitwise_and(self.f1>int(self.inputLowBound.get()),self.f1<int(self.inputUpBound.get()))]=d_gauss
			
		self.a2 = plotIAF(self.f1,d,"purple",self.a2,"IAF")
		
		# Get dat IAF val
		d_search = d[bitwise_and(self.f1>int(self.inputLowBound.get()),self.f1<int(self.inputUpBound.get()))]
		f_search = self.f1[bitwise_and(self.f1>int(self.inputLowBound.get()),self.f1<int(self.inputUpBound.get()))]
		f_idx = argmax(d_search)
		print f_search[f_idx]
		self.inputIAF.delete(0, END)
		self.inputIAF.insert(0, "%.2f" % (f_search[f_idx]))
		
		
		# Autoscale		
		self.Dmin = amin(d_search)-2
		self.Dmax = amax(d_search)+2
		self.a2.set_ylim(self.Dmin,self.Dmax) # little modifier to differentiate the peak
		
		# IAF position
		self.a2.vlines(f_search[f_idx],self.Dmin,self.Dmax,color="Cyan")
		# Search limits
		self.a2.vlines(int(self.inputLowBound.get()),self.Dmin,self.Dmax,linestyles=":",linewidth=0.25)
		self.a2.vlines(int(self.inputUpBound.get()),self.Dmin,self.Dmax,linestyles=":",linewidth=0.25)
		
		self.fig.show()
		
		# Set filter configs
	
	def goTime(self):
		print "ITS GO TIME!"
		print self.filename1
		print self.filename2
		self.getChannelList()
		print self.chSelect
		print self.chOther
		
		self.f1,self.P1 = computeSpectrum(self.filename1,int(self.inputFs.get()),int(self.inputWinSize.get()),int(self.inputOverlap.get()))
		self.f2,self.P2 = computeSpectrum(self.filename2,int(self.inputFs.get()),int(self.inputWinSize.get()),int(self.inputOverlap.get()))
		
		# Plotting time
		self.a1.cla()
		self.a1 = plotSpectrum(self.f1,self.P1,"blue",self.a1,"Power spectrum",self.chSelect,self.chOther)
		self.a1 = plotSpectrum(self.f2,self.P2,"red",self.a1,"Power spectrum",self.chSelect,self.chOther)
		
		# Trying to autoscale
		P1_ROI = 20*log10(self.P1[:,bitwise_and(self.f1>1,self.f1<20)])
		P2_ROI = 20*log10(self.P2[:,bitwise_and(self.f2>1,self.f2<20)])
		
		self.Pmax = amax([amax(P1_ROI),amax(P2_ROI)])
		self.Pmin = amin([(amin(P1_ROI)),amin(P2_ROI)])
		self.a1.set_ylim(self.Pmin,self.Pmax)
		# Autoscale success :>
		self.fig.show()
		
		
	def writeFilters(self):
		f_theta = open(self.nameO.get()+"/theta.f","w")
		f_theta.write("<OpenViBE-SettingsOverride>\n\t<SettingValue>Butterworth</SettingValue>\n\t<SettingValue>Band pass</SettingValue>\n\t<SettingValue>4</SettingValue>\n\t<SettingValue>{:.2f}</SettingValue>\n\t<SettingValue>{:.2f}</SettingValue>\n\t<SettingValue>0.5</SettingValue>\n</OpenViBE-SettingsOverride>".format( float(self.inputThetaLow.get()), float(self.inputThetaHigh.get())  ) )
		f_theta.close()
		
		f_beta = open(self.nameO.get()+"/beta.f","w")
		f_beta.write("<OpenViBE-SettingsOverride>\n\t<SettingValue>Butterworth</SettingValue>\n\t<SettingValue>Band pass</SettingValue>\n\t<SettingValue>4</SettingValue>\n\t<SettingValue>{:.2f}</SettingValue>\n\t<SettingValue>{:.2f}</SettingValue>\n\t<SettingValue>0.5</SettingValue>\n</OpenViBE-SettingsOverride>".format( float(self.inputBetaLow.get()), float(self.inputBetaHigh.get())  ) )
		f_beta.close()
		
		f_smr = open(self.nameO.get()+"/smr.f","w")
		f_smr.write("<OpenViBE-SettingsOverride>\n\t<SettingValue>Butterworth</SettingValue>\n\t<SettingValue>Band pass</SettingValue>\n\t<SettingValue>4</SettingValue>\n\t<SettingValue>{:.2f}</SettingValue>\n\t<SettingValue>{:.2f}</SettingValue>\n\t<SettingValue>0.5</SettingValue>\n</OpenViBE-SettingsOverride>".format( float(self.inputSMRLow.get()), float(self.inputSMRHigh.get())  ) )
		f_smr.close()
		
		
	def printFigureToFile(self):
		self.fig1.savefig(self.nameO.get() + "/IAF_spectrum.png")
	def askOpenFile1(self):
		ftypes = [('asdadfh', '*.txt'), ('All files','*')]
		dlg = tkFileDialog.Open(self, filetypes=ftypes)
		self.filename1 = dlg.show()	
		self.nameF1.delete(0, END)
		self.nameF1.insert(0, self.filename1)
		
	def askOpenFile2(self):
		ftypes = [('asdadfh', '*.txt'), ('All files','*')]
		dlg = tkFileDialog.Open(self, filetypes=ftypes)
		self.filename2 = dlg.show()
		self.nameF2.delete(0, END)
		self.nameF2.insert(0, self.filename2)
		
	def askOutputDirectory(self):
		dlg = tkFileDialog.askdirectory()
		#self.outputdir = dlg.show()
		self.outputdir = dlg
		self.nameO.delete(0,END)
		self.nameO.insert(0,self.outputdir)
		
def computeSpectrum(filename,srate,winsize,ol):
	# print(filename)
	print srate
	data = genfromtxt(filename)
	data = data[:,0:8]
	print shape(data)
	P=zeros((8,int(round(winsize/2)+1)));
	for k in range(0,8):
		# Calculate power spectral density
		p = matplotlib.mlab.psd(data[:,k],NFFT=winsize,Fs=srate,noverlap=ol)
		p = asarray(p)
		print p.shape
		P[k,:]=p[0,:]
	f = p[1,:]
	return (f,P)

def plotSpectrum(f,P,cstr,sp,titlestr,ch_select,ch_other):

	for k in range(0,ch_other.shape[0]):
		sp.plot(f,20*log10(P[ch_other[k],:]),color="gray",linestyle="-",linewidth=0.10)
	
	for k in range(0,ch_select.shape[0]):
		sp.plot(f,20*log10(P[ch_select[k],:]),color=cstr,linestyle="-",linewidth=0.25)

	sp.plot(f,20*log10(mean(P[ch_select.astype(int),:],axis=0)),color=cstr,linestyle="-",linewidth=1.25)
	sp.set_title(titlestr)
	sp.set_xlabel('Frequency [Hz]')
	sp.set_ylabel('Power [dB]')
	sp.set_xlim(1,30)
	sp.set_ylim(100,200)
	return sp

def plotIAF(f,D,cstr,sp,titlestr):
	sp.cla()
	sp.plot(f,D,color=cstr,linestyle="-",linewidth=1.25)
	sp.set_title(titlestr)
	sp.set_xlabel("Frequency [Hz]")
	sp.set_ylabel("Power [dB]")
	sp.set_xlim(1,30)
	
	return sp
	
def main():
  
	# font = {'family' : 'normal',
        # 'weight' : 'normal',
        # 'size'   : 6}

	# matplotlib.rc('font', **font)
	
	
	root = Tk()
	ex = Example(root)
	root.geometry("1200x600+10+10")
	root.mainloop()  

if __name__ == '__main__':
	main()  