import React, {Component} from 'react';
import Graficas from './components/Graficas';
import Datos from './components/Datos';
import { BrowserRouter as Router, Switch, Route} from "react-router-dom";
import Navbar from './components/Navbar';
import Procesos from './components/Procesos';

export default class App extends Component{

  constructor(props){
      super(props);
      this.urlG = "localhost";
      this.state = {
            arregloRamA : [0,0,0,0,0],
            arregloRamB : [0,0,0,0,0],
            arregloCpuA : [0,0,0,0,0],
            arregloCpuB : [0,0,0,0,0],
            ramA:0,
            ramB:0,
            ramC:0,
            cpuA:0,
            cpuB:0,
            ptotal:0,
            pejecucion:0,
            psuspendidos:0,
            pdetenidos:0,
            pzombies:0,
            ppid: [],
            pnombre: [],
            pusuario: [],
            pusuarionombre: [],
            pestado: [],
            pram: [],
            DatosA: [{"autor":"Sergio", "nota":"RAM"}],
            DatosB: [{"autor":"Sergio", "nota":"CPU"}]
      }
      this.RamTotal = 0;
      this.RamLibre = 0;
      this.RamUsada = 0;
      this.CpuTotal = 0;
      this.PTotal = 0;
      this.PEjecucion = 0;
      this.PSuspendidos = 0;
      this.PDetenidos = 0;
      this.PZombies = 0;
      this.PPid = [];
      this.PNombre = [];
      this.PUsuario = [];
      this.PUsuarioNombre = [];
      this.PEstado = [];
      this.PRam = [];
  }

  async USAGE_RAM(){
      var url = "http://".concat(this.urlG,":3001","/ram");
      fetch(url)
      .then(res => res.json())
      .then((data) => {
          this.RamTotal = data.Total;
          this.RamLibre = data.Total-data.Libre;
          this.RamUsada = data.Usada;
      })
      var arregloRamA1 = this.state.arregloRamA;
      arregloRamA1[4] = arregloRamA1[3];
      arregloRamA1[3] = arregloRamA1[2];
      arregloRamA1[2] = arregloRamA1[1];
      arregloRamA1[1] = arregloRamA1[0];
      arregloRamA1[0] = parseInt(this.RamUsada, 10);
      var result = arregloRamA1.map((arregloRamA1)=>arregloRamA1*1);
      this.setState({arregloRamA:result, ramA: arregloRamA1[0]}); 
      this.setState({arregloRamA:result, ramB: this.RamTotal}); 
      this.setState({arregloRamA:result, ramC: this.RamLibre}); 
  }

  async USAGE_CPU(){
    var url = "http://".concat(this.urlG,":3001","/cpu")
    fetch(url)
      .then(res => res.json())
      .then((data) => {
          this.CpuTotal = data.Total;
      })
    var arregloCpu = this.state.arregloCpuA;
    arregloCpu[4] = arregloCpu[3];
    arregloCpu[3] = arregloCpu[2];
    arregloCpu[2] = arregloCpu[1];
    arregloCpu[1] = arregloCpu[0];
    arregloCpu[0] = parseInt(this.CpuTotal, 10);
    var result = arregloCpu.map((arregloCpu)=>arregloCpu*1);
    this.setState({arregloCpuA:result, cpuA:arregloCpu[0] });   
}

    async USAGE_PROCESS(){
        var url = "http://".concat(this.urlG,":3001","/process")
        fetch(url)
        .then(res => res.json())
        .then((data) => {
            this.PTotal = data.ProcessInfo.Total;
            this.PEjecucion = data.ProcessInfo.Ejecucion;
            this.PSuspendidos = data.ProcessInfo.suspendidos;
            this.PDetenidos = data.ProcessInfo.detenidos;
            this.PZombies = data.ProcessInfo.zombies;
            console.log(data.ProcessList);
            this.getProcess(data.ProcessList);
        })
        this.setState({ptotal: this.PTotal});
        this.setState({pejecucion: this.PEjecucion});
        this.setState({psuspendidos: this.PSuspendidos});
        this.setState({pdetenidos: this.PDetenidos});
        this.setState({pzombies: this.PZombies});
        this.setState({ppid: this.PPid});
        this.setState({pnombre: this.PNombre});
        this.setState({pusuario: this.PUsuario});
        this.setState({pusuarionombre: this.PUsuarioNombre});
        this.setState({pestado: this.PEstado});
        this.setState({pram: this.PRam});
    }

  componentWillMount(){
      setInterval(() => {
          this.USAGE_RAM();
          this.USAGE_CPU();
          this.USAGE_PROCESS();
      }, 5000);
  }

  getProcess(Padre = []) {
    let i = 0;
    let j = 0;
    while(i<Padre.length){
        this.PPid[j]=Padre[i].Pid;
        this.PNombre[j]=Padre[i].Nombre;
        this.PUsuario[j]=Padre[i].Usuario;
        this.PUsuarioNombre[j]=Padre[i].Usuarionombre;
        this.PEstado[j]=Padre[i].Estado;
        this.PRam[j]=Padre[i].ram;
        /*
        if (Padre[i].hijos.length>0){
            j = j + 6;
            for(let k = 0; k<Padre[i].hijos.length; k++){
                j = j + this.getChild(Padre[i].hijos[k], j);
            }
        }else{
            j = j + 6;
        }*/
        j = j + 1;
        i = i + 1;
    }
  }

  getChild(Padre = [], j){
    let i = 0;
    while(i<Padre.length){
        this.PPid[j]=Padre[i].Pid+"\n";
        this.PNombre[j+1]=Padre[i].Nombre+"\n";
        this.PUsuario[j+2]=Padre[i].Usuario+"\n";
        this.PUsuarioNombre[j+3]=Padre[i].Usuarionombre+"\n";
        this.PEstado[j+4]=Padre[i].Estado+"\n";
        this.PRam[j+5]=Padre[i].ram+"\n";
        if (Padre[i].hijos.length>0){
            j = j + 6;
            for(let k = 0; k<Padre[i].hijos.length; k++){
                j = j + this.getChild(Padre[i].hijos[k], j);
            }
        }else{
            j = j + 6;
        }
        i = i + 1;
    }
    return j;
  }

  render(){
      return(
          <Router>
              <div className="App">
                  <Navbar >
                  </Navbar>
                  <Switch>
                      <Route path= "/charts">
                          <Graficas   ramA={this.state.ramA} ramB={this.state.ramB} ramC={this.state.ramC} cpuA={this.state.cpuA} cpuB={this.state.cpuB}
                                      arregloRamA={this.state.arregloRamA} arregloRamB = {this.state.arregloRamB} arregloCpuA = {this.state.arregloCpuA} arregloCpuB = {this.state.arregloCpuB} />
                      </Route>
                      <Route path="/dataA">
                          <Datos data={this.state.DatosA} name="A"/>
                      </Route>
                      <Route path="/procesos">
                          <Procesos data={this.state.DatosB} ptotal={this.state.ptotal} pejecucion={this.state.pejecucion} psuspendidos={this.state.psuspendidos}
                                    pdetenidos={this.state.pdetenidos} pzombies={this.state.pzombies} ppid={this.state.ppid} pnombre={this.state.pnombre}
                                    pusuarion={this.state.pusuario} pusuarionombre={this.state.pusuarionombre} pestado={this.state.pestado} pram={this.state.pram}/> 
                      </Route>
                  </Switch>
              </div>
          </Router>
      )
  }
}
/*
export class Process {
	Pid = "";    
	Nombre = "";      
	Usuario = "";      
	Usuarionombre = "";   
    Hijos = [];
}
*/