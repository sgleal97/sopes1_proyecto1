import React, {Component} from 'react';
import { Line } from 'react-chartjs-2';
import BorderWrapper from 'react-border-wrapper';
import "bootstrap/dist/css/bootstrap.min.css";

class Procesos extends Component{
    constructor(props){
        super(props);
        this.state = {
            data: []
        }
    }
    render(){
        this.state.data = this.props.data;
        return(
            <div>
                <div>
                <h1>Procesos</h1>
                </div>
                <div>
                    <table className="table">
                        <thead className="bg-info">
                            <tr>
                                <th>Total</th>
                                <th>Ejecucion</th>
                                <th>Suspendidos</th>
                                <th>Detenidos</th>
                                <th>Zombies</th>
                            </tr>
                        </thead>
                        <tbody>
                        <tr>
                                <th>{this.props.ptotal}</th>
                                <th>{this.props.pejecucion}</th>
                                <th>{this.props.psuspendidos}</th>
                                <th>{this.props.pdetenidos}</th>
                                <th>{this.props.pzombies}</th>
                            </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        )
    }

}

export default Procesos