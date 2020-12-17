import React, {Component} from 'react';
import { Line } from 'react-chartjs-2';

class Graficas extends Component{
    constructor(props){
        super(props);
        this.state = {
            ram: {
                    labels:["Estado 1","Estado 2","Estado 3","Estado 4","Estado 5"],
                    datasets:[
                        {
                            label: "Ram Servidor A",
                            backgroundColor:"rgba(31,96,236,0.75)",
                            data: [0]
                        },
                        {
                            label: "Ram BASE",
                            backgroundColor:"rgba(0,0,0,0.75)",
                            data:[0,0,0,0,0]
                        }
                    
                    ]
            },
            cpu :{
                    labels:["Estado 1","Estado 2","Estado 3","Estado 4","Estado 5"],
                    datasets:[
                        {
                            label: "CPU Servidor A",
                            backgroundColor:"rgba(72,5,182,0.75)",
                            data:[0]
                        },
                        {
                            label: "CPU BASE",
                            backgroundColor:"rgba(0,0,255,0.75)",
                            data:[0,0,0,0,0]
                        }
                    ]
            }
        }
    }
    render(){

        this.state.ram.datasets[0].data = this.props.arregloRamA;
        this.state.ram.datasets[1].data = this.props.arregloRamB;
        this.state.cpu.datasets[0].data = this.props.arregloCpuA;
        this.state.cpu.datasets[1].data = this.props.arregloCpuB;
        return(
            <div>
                <h1>Monitor de RAM</h1>
                <div style={{position:"center",width: 1000, height:1000}}>
                    <h3>USO DE RAM</h3>
                    <Line
                    options = {{
                        responsive:true
                    }}
                    data ={this.state.ram}
                    />
                    <h5>RAM Total: {this.props.ramB} MB</h5>
                    <h5>RAM Libre: {this.props.ramC} MB</h5>
                    <h5>Porcentaje: {this.props.ramA}%</h5>
                    
                </div>
                <h1>Monitor de CPU</h1>
                <div style={{position:"center",width: 1000, height:1000}}>
                    <h3>USO DE CPU</h3>
                    <Line
                    options = {{
                        responsive:true
                    }}
                    data ={this.state.cpu}
                    />
                    <h5>Porcentaje: {this.props.cpuA}%</h5>
                </div>
            </div>
        )
    }
}

export default Graficas