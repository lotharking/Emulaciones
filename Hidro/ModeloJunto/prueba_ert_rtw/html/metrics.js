function CodeMetrics() {
	 this.metricsArray = {};
	 this.metricsArray.var = new Array();
	 this.metricsArray.fcn = new Array();
	 this.metricsArray.var["rtDW"] = {file: "C:\\Users\\Hp 14\\Google Drive\\Universidad\\tesis\\trabajo de grado\\modelos\\Hidrico\\prueba_ert_rtw\\prueba.c",
	size: 328};
	 this.metricsArray.var["rtM_"] = {file: "C:\\Users\\Hp 14\\Google Drive\\Universidad\\tesis\\trabajo de grado\\modelos\\Hidrico\\prueba_ert_rtw\\prueba.c",
	size: 369};
	 this.metricsArray.var["rtU"] = {file: "C:\\Users\\Hp 14\\Google Drive\\Universidad\\tesis\\trabajo de grado\\modelos\\Hidrico\\prueba_ert_rtw\\prueba.c",
	size: 8};
	 this.metricsArray.var["rtY"] = {file: "C:\\Users\\Hp 14\\Google Drive\\Universidad\\tesis\\trabajo de grado\\modelos\\Hidrico\\prueba_ert_rtw\\prueba.c",
	size: 56};
	 this.metricsArray.fcn["cos"] = {file: "C:\\Program Files\\MATLAB\\R2017b\\sys\\lcc\\include\\math.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["prueba_initialize"] = {file: "C:\\Users\\Hp 14\\Google Drive\\Universidad\\tesis\\trabajo de grado\\modelos\\Hidrico\\prueba_ert_rtw\\prueba.c",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["prueba_step"] = {file: "C:\\Users\\Hp 14\\Google Drive\\Universidad\\tesis\\trabajo de grado\\modelos\\Hidrico\\prueba_ert_rtw\\prueba.c",
	stack: 56,
	stackTotal: 56};
	 this.metricsArray.fcn["sin"] = {file: "C:\\Program Files\\MATLAB\\R2017b\\sys\\lcc\\include\\math.h",
	stack: 0,
	stackTotal: 0};
	 this.getMetrics = function(token) { 
		 var data;
		 data = this.metricsArray.var[token];
		 if (!data) {
			 data = this.metricsArray.fcn[token];
			 if (data) data.type = "fcn";
		 } else { 
			 data.type = "var";
		 }
	 return data; }; 
	 this.codeMetricsSummary = '<a href="prueba_metrics.html">Global Memory: 761(bytes) Maximum Stack: 56(bytes)</a>';
	}
CodeMetrics.instance = new CodeMetrics();
