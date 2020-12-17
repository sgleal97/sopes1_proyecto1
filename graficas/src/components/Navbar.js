import React from 'react';
import {NavLink, withRouter}  from 'react-router-dom'
class Navbar extends React.Component {
    getNavLinkClass = (path) => {
        return path;
    }
    render() {
        return (
            <nav className="navbar navbar-inverse" >
                <div className="container-fluid">
                    <style>{"body { background-color: white; }"}</style>
                    <div className="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                        <ul className="nav navbar-nav navbar-right">
                           <li className={this.getNavLinkClass("/charts")}><NavLink to="/charts">Graficas</NavLink></li>
                           <li className={this.getNavLinkClass("/dataA")}><NavLink to="/dataA">Servidor A</NavLink></li>
                           <li className={this.getNavLinkClass("/procesos")}><NavLink to="/procesos">Procesos</NavLink></li>
                        </ul>
                    </div>
                </div>
            </nav>
        )
    }
};
Navbar = withRouter(Navbar);
export default Navbar;