import React, { Component } from 'react';
import BorderWrapper from 'react-border-wrapper'

class Datos extends Component{
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
                <h1>Datos Servidor {this.props.name}</h1>
                {
                    this.state.data.map(function(nota){
                            return (
                                <BorderWrapper>
                                    <article style={{textAlign: "left"}}>
                                        <h3>{nota.autor}</h3>
                                        <p>{nota.nota}</p>
                                    </article>
                                </BorderWrapper>
                            )
                        
                        })
                }
                
            </div>
        )
    }

}

export default Datos